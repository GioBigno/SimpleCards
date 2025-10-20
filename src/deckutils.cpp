#include "deckutils.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QRegularExpression>
#include <map>
#include <tuple>

DeckUtils::DeckUtils(QObject *parent)
    : QObject{parent}
{}

QVariantList DeckUtils::getAvailableDecks() const
{
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QFileInfoList fileList = dataDir.entryInfoList(QStringList("*.json"), QDir::Files);

	std::sort(fileList.begin(), fileList.end(), [](const QFileInfo &a, const QFileInfo &b){
		return a.lastModified() > b.lastModified();
	});

	QVariantList ret;
	ret.reserve(fileList.size());
	for(const QFileInfo &fileInfo : fileList){
		QVariantMap map;
		map["file_path"] = fileInfo.absoluteFilePath();
		map["base_name"] = fileInfo.baseName();
		ret << map;
	}

	return ret;
}

DeckModel* DeckUtils::getDeckModel() const
{
	return m_deckModel.get();
}

QString DeckUtils::loadDeck(const QString& fileName, DeckMode mode)
{
	auto result = jsonFromFile(fileName).and_then(deckFromJson);
	if(!result){
		qWarning() << "[deckutils]" << result.error();
		return result.error();
	}

	m_deckModel = std::make_unique<DeckModel>(std::move(result.value()), mode, this);
	m_deckFilePath = fileName;
	emit deckModelChanged();
	return "";
}

void DeckUtils::saveLoadedDeck()
{
	if(m_deckFilePath.isEmpty()){
		qCritical() << "[deckutils] Error, no deck loaded";
		return;
	}

	bool res = jsonToFile(jsonFromDeck(m_deckModel->getDeck()), m_deckFilePath);
	emit availableDecksChanged();

	if(!res)
		qCritical() << "[deckutils] Error saving deck to file";
}

void DeckUtils::deleteLoadedDeck()
{
	if(m_deckFilePath.isEmpty()){
		qCritical() << "[deckutils] Error, no deck loaded";
		return;
	}

	m_deckModel.reset();
	emit deckModelChanged();
	if(!QFile::moveToTrash(m_deckFilePath))
		QFile::remove(m_deckFilePath);
	m_deckFilePath = "";
	emit availableDecksChanged();
}

void DeckUtils::deleteDeck(const QString& filePath)
{
	QFile::remove(filePath);
	emit availableDecksChanged();
}

QString DeckUtils::createEmptyDeckFile()
{
	QString deckName("New Deck");
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	
	QString fileName = QString("%1/%2.json")
				 .arg(dataDir.absolutePath())
				 .arg(sanitizeFileName(deckName));
	fileName = uniqueFileName(std::move(fileName));

	Deck emptyDeck(deckName);
	
	if(!jsonToFile(jsonFromDeck(emptyDeck), fileName))
		qCritical() << "[deckutils] Error saving deck to file";

	emit availableDecksChanged();
	return fileName;
}
	
void DeckUtils::changeLoadedDeckFileName(const QString& deckName)
{
	QFileInfo fileInfo(m_deckFilePath);

	if(!fileInfo.exists())
		qCritical() << "[deckutils] File does not exist:" <<m_deckFilePath;

	QString sanitizedName = sanitizeFileName(deckName);
	QString newFilePath = QString("%1/%2.json")
				    .arg(fileInfo.absolutePath())
				    .arg(sanitizedName);

	if(m_deckFilePath == newFilePath)
		return;

	newFilePath = uniqueFileName(std::move(newFilePath));

	QFile file(m_deckFilePath);
	if(QFile::rename(m_deckFilePath, newFilePath)){
		emit availableDecksChanged();
		m_deckFilePath = newFilePath; 
	}
}

std::expected<QJsonDocument, QString> DeckUtils::jsonFromFile(const QString& jsonFilename)
{
	QJsonDocument jsonDoc;

	QFile jsonFile(jsonFilename);
	if(!jsonFile.open(QIODevice::ReadOnly))
		return std::unexpected{"Error parsing json: " + jsonFile.errorString()};

	QJsonParseError jsonErr;
	jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &jsonErr);
	jsonFile.close();
	if(jsonDoc.isNull())
		return std::unexpected{"Error parsing json: " + jsonErr.errorString()};

	return jsonDoc;
}

bool DeckUtils::jsonToFile(const QJsonDocument& doc, const QString& fileName)
{
	QString tempPath = fileName + ".tmp";
	QFile tempFile(tempPath);

	if(!tempFile.open(QIODevice::WriteOnly)){
		qCritical() << "[deckutils] Error: " << tempFile.errorString();
		return false;
	}

	tempFile.write(doc.toJson(QJsonDocument::Indented));
	tempFile.close();

	QFile::remove(fileName);
	return QFile::rename(tempPath, fileName);
}

std::expected<Deck, QString> DeckUtils::deckFromJson(const QJsonDocument& jsonDoc)
{	
	QString prefix("Error parsing json: ");

	QJsonObject deckObj = jsonDoc.object();
	if(deckObj.isEmpty())
		return std::unexpected{prefix + "main component is not an object"};

	QJsonValue deckName_val = deckObj.value("deck_name");
	if(!deckName_val.isString())
		return std::unexpected{prefix + "deck_name not found"};
	
	QString deckName = deckName_val.toString();
	if(deckName.isEmpty())
		return std::unexpected{prefix + "deck_name is not a string"};

	QJsonValue cardsArr_val = deckObj.value("cards");
	if(!cardsArr_val.isArray())
		return std::unexpected{prefix + "cards not found"};

	QJsonArray cardsArr = cardsArr_val.toArray();
	std::vector<Card> cards;
	cards.reserve(cardsArr.size());
	
	for(const auto& card : cardsArr){
		QJsonObject cardObj = card.toObject();
		if(cardObj.isEmpty())
			return std::unexpected{prefix + "card object is empty"};

		QJsonValue question_val = cardObj.value("question");
		if(!question_val.isString())
			return std::unexpected{prefix + "question is not a string"};

		QString question = question_val.toString();

		QJsonValue answer_val = cardObj.value("answer");
		if(!answer_val.isString())
			return std::unexpected{prefix + "answer is not a string"};

		QString answer = answer_val.toString();

		QJsonValue creationDate_val = cardObj.value("date_creation");
		if(!creationDate_val.isString())
			return std::unexpected{prefix + "date_creation is not a string"};

		QDate creationDate = QDate::fromString(creationDate_val.toString(), Qt::ISODate);
		if(!creationDate.isValid())
			return std::unexpected{prefix + "date_creation is not a valid date"};

		QJsonValue nextReviewDate_val = cardObj.value("date_next_review");
		if(!nextReviewDate_val.isString())
			return std::unexpected{prefix + "date_next_review is not a string"};

		QDate nextReviewDate = QDate::fromString(nextReviewDate_val.toString(), Qt::ISODate);
		if(!nextReviewDate.isValid())
			return std::unexpected{prefix + "date_next_review is not a valid date"};

		QJsonValue ease_val = cardObj.value("ease");
		if(!ease_val.isDouble())
			return std::unexpected{prefix + "ease is not a double"};

		double ease = ease_val.toDouble();

		QJsonValue interval_val = cardObj.value("interval");
		if(!interval_val.isDouble())
			return std::unexpected{prefix + "interval is not an int"};

		size_t interval = interval_val.toInt();

		QJsonValue repetitions_val = cardObj.value("repetitions");
		if(!repetitions_val.isDouble())
			return std::unexpected{prefix + "repetitions is not an int"};

		size_t repetitions = repetitions_val.toInt();

		cards.emplace_back(question, answer, creationDate, nextReviewDate, ease, interval, repetitions);
	}

	QJsonValue statsArr_val = deckObj.value("stats");
	if(!statsArr_val.isArray())
		return std::unexpected{prefix + "stats not found"};

	QJsonArray statsArr = statsArr_val.toArray();
	std::map<QDate, std::tuple<int, int, int>> master_history;
	
	for(const auto& log : statsArr){
		QJsonObject logObj = log.toObject();
		if(logObj.isEmpty())
			return std::unexpected{prefix + "stat object is empty"};

		QJsonValue logDate_val = logObj.value("date");
		if(!logDate_val.isString())
			return std::unexpected{prefix + "date not found"};

		QDate logDate = QDate::fromString(logDate_val.toString(), Qt::ISODate);
		if(!logDate.isValid())
			return std::unexpected{prefix + "date is not a valid date"};

		QJsonValue n_new_val = logObj.value("new");
		if(!n_new_val.isDouble())
			return std::unexpected{prefix + "new is not an int"};

		int n_new = n_new_val.toInt();

		QJsonValue n_learning_val = logObj.value("learning");
		if(!n_learning_val.isDouble())
			return std::unexpected{prefix + "learning is not an int"};

		int n_learning = n_learning_val.toInt();

		QJsonValue n_mastered_val = logObj.value("mastered");
		if(!n_mastered_val.isDouble())
			return std::unexpected{prefix + "mastered is not an int"};

		int n_mastered = n_mastered_val.toInt();

		master_history[logDate] = std::make_tuple(n_new, n_learning, n_mastered);
	}
	return Deck(std::move(deckName), std::move(cards), master_history);
}

QJsonDocument DeckUtils::jsonFromDeck(const Deck& deck)
{
	QJsonObject mainObj;
	mainObj.insert("deck_name", deck.getName());
	
	QJsonArray cardsArray;
	for(const Card& card : deck.getCards()){
		QJsonObject cardObj;
		cardObj.insert("question", QJsonValue(card.getQuestion()));
		cardObj.insert("answer", QJsonValue(card.getAnswer()));
		cardObj.insert("date_creation", QJsonValue(card.getCreationDate().toString(Qt::ISODate)));
		cardObj.insert("ease", QJsonValue(card.getEase()));
		cardObj.insert("interval", QJsonValue(static_cast<qint64>(card.getInterval())));
		cardObj.insert("repetitions", QJsonValue(static_cast<qint64>(card.getRepetitions())));
		cardObj.insert("date_next_review", QJsonValue(card.getNextReviewDate().toString(Qt::ISODate)));
		cardsArray.push_back(cardObj);
	}
	mainObj.insert("cards", cardsArray);

	QJsonArray statsArray;
	for(auto const& [date, level] : deck.getMasterHistory()){
		QJsonObject logObj;
		logObj.insert("date", QJsonValue(date.toString(Qt::ISODate)));
		logObj.insert("new", QJsonValue(std::get<0>(level)));
		logObj.insert("learning", QJsonValue(std::get<1>(level)));
		logObj.insert("mastered", QJsonValue(std::get<2>(level)));
		statsArray.push_back(logObj);
	}
	mainObj.insert("stats", statsArray);

	QJsonDocument doc(mainObj);
	return doc;
}

QString DeckUtils::sanitizeFileName(QString str) const
{
	str = str.trimmed();

	// Replace one or more spaces with a single underscore
	str.replace(QRegularExpression("\\s+"), "_");
	// Remove invalid filename characters
	str.remove(QRegularExpression("[/\\\\:*?\"<>|]"));
	
	return str;
}

QString DeckUtils::uniqueFileName(QString str) const
{
	//given a file name it returns a new name that doesn't exists
	QFileInfo fileInfo(str);
	QString dirPath = fileInfo.absolutePath();
	QString baseName = fileInfo.baseName();
	QString extension = fileInfo.completeSuffix();
	size_t counter = 1;

	while(QFile::exists(str)){
		str = QString("%1/%2(%3)")
			.arg(dirPath)
			.arg(baseName)
			.arg(counter);

		if(!extension.isEmpty())
			str += "." + extension;

		counter++;
	}

	return str;
}

