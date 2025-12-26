#include "deckutils.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QRegularExpression>
#include <map>
#include <tuple>

#if defined(Q_OS_WASM)
	#include <emscripten.h>
#endif

DeckUtils::DeckUtils(QObject *parent)
    : QObject{parent}
{}

QVariantList DeckUtils::getAvailableDecks() const
{
	QFileInfoList fileList = dataDir.entryInfoList({QString("*") + DeckUtils::DECK_FILE_SUFFIX}, QDir::Files);

	std::sort(fileList.begin(), fileList.end(), [](const QFileInfo &a, const QFileInfo &b){
		return a.lastModified() > b.lastModified();
	});

	QVariantList ret;
	ret.reserve(fileList.size());
    	for(const QFileInfo &fileInfo : std::as_const(fileList)){
		QVariantMap map;
		map["deck_file_path"] = fileInfo.absoluteFilePath();
		map["base_name"] = fileInfo.baseName();
		ret << map;
	}

	return ret;
}

DeckModel* DeckUtils::getDeckModel() const
{
	return m_deckModel.get();
}

void DeckUtils::setDataDir(const QUrl& url)
{
	dataDir = QDir(url.toLocalFile());
	emit availableDecksChanged();
}

QUrl DeckUtils::getDataDir() const
{
	return QUrl(dataDir.absolutePath());
}

QString DeckUtils::loadDeck(const QString& deckFileName, DeckMode mode)
{
	//TODO improve this spaghetti code function
	std::expected<Deck, QString> result = jsonFromFile(deckFileName).and_then(deckFromJson);
	if(!result){
		qWarning() << "[deckutils]" << result.error();
		return result.error();
	}

	const QString statsFileName = statsFileNameFromDeckFileName(deckFileName);
	if(QFile::exists(statsFileName)){
		std::expected<QJsonDocument, QString> statsJson = jsonFromFile(statsFileName);
		
		if(!statsJson){
			qWarning() << "[deckutils]" << statsJson.error();
			return statsJson.error();
		}

		result = deckAddStatsFromJson(std::move(result.value()), statsJson.value());
	
		if(!result){
			qWarning() << "[deckutils]" << result.error();
			return result.error();
		}
	}

	m_deckModel = std::make_unique<DeckModel>(std::move(result.value()), mode, this);
	m_deckFilePath = deckFileName;
	emit deckModelChanged();
	return "";
}

void DeckUtils::saveLoadedDeck()
{
	if(m_deckFilePath.isEmpty()){
		qCritical() << "[deckutils] Error, no deck loaded";
		return;
	}

	if(!jsonToFile(deckJsonFromDeck(m_deckModel->getDeck()), m_deckFilePath))
		qCritical() << "[deckutils] Error saving deck to file";

	emit availableDecksChanged();
	
	if(!jsonToFile(statsJsonFromDeck(m_deckModel->getDeck()), statsFileNameFromDeckFileName(m_deckFilePath)))
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
	deleteDeck(m_deckFilePath);
}

void DeckUtils::deleteDeck(const QString& deckFilePath)
{
	if(!QFile::moveToTrash(deckFilePath))
		QFile::remove(deckFilePath);
	const QString statsFilePath = statsFileNameFromDeckFileName(deckFilePath);
	if(!QFile::moveToTrash(statsFilePath))
		QFile::remove(statsFilePath);
	emit availableDecksChanged();

#if defined(Q_OS_WASM)
	EM_ASM(
		FS.syncfs(false, function (err) {
			if(err)console.log("Error syncing FS: " + err);
		});
	);
#endif
}

QString DeckUtils::createEmptyDeckFile()
{
	QString deckName("New Deck");
	
	QString fileName = QString("%1/%2%3")
                 .arg(dataDir.absolutePath(),
                      sanitizeFileName(deckName),
			    DeckUtils::DECK_FILE_SUFFIX);
	fileName = uniqueFileName(std::move(fileName));

	Deck emptyDeck(deckName);
	if(!jsonToFile(deckJsonFromDeck(emptyDeck), fileName))
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
	QString newDeckFilePath = QString("%1/%2%3")
                    .arg(fileInfo.absolutePath(),
                         sanitizedName,
				 DeckUtils::DECK_FILE_SUFFIX);

	if(m_deckFilePath == newDeckFilePath)
		return;

	newDeckFilePath = uniqueFileName(std::move(newDeckFilePath));
	const QString old_statsFilePath = statsFileNameFromDeckFileName(m_deckFilePath);
	const QString new_statsFilePath = statsFileNameFromDeckFileName(newDeckFilePath);

	if(!QFile::rename(m_deckFilePath, newDeckFilePath))
		return;
	
	emit availableDecksChanged();
	m_deckFilePath = newDeckFilePath; 

	QFile::rename(old_statsFilePath, new_statsFilePath);

#if defined(Q_OS_WASM)
	EM_ASM(
		FS.syncfs(false, function (err) {
			if(err)console.log("Error syncing FS: " + err);
		});
	);
#endif
}

QString DeckUtils::statsFileNameFromDeckFileName(const QString& deckFileName)
{
	if(deckFileName.endsWith(DeckUtils::DECK_FILE_SUFFIX, Qt::CaseInsensitive))
		return deckFileName.left(deckFileName.length() - DeckUtils::DECK_FILE_SUFFIX.length()) + DeckUtils::STATS_FILE_SUFFIX;

	return deckFileName;
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
	bool ret = QFile::rename(tempPath, fileName);

#if defined(Q_OS_WASM)
	EM_ASM(
		FS.syncfs(false, function (err) {
			if(err)console.log("Error syncing FS: " + err);
		});
	);
#endif

	return ret;
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
	
    	for(const auto& card : std::as_const(cardsArr)){
		QJsonObject cardObj = card.toObject();
		if(cardObj.isEmpty())
			return std::unexpected{prefix + "card object is empty"};
		
		QJsonValue id_val = cardObj.value("id");
		if(!id_val.isDouble())
			return std::unexpected{prefix + "id is not a number"};

		int id = id_val.toInt();

		QJsonValue question_val = cardObj.value("question");
		if(!question_val.isString())
			return std::unexpected{prefix + "question is not a string"};

		QString question = question_val.toString();

		QJsonValue answer_val = cardObj.value("answer");
		if(!answer_val.isString())
			return std::unexpected{prefix + "answer is not a string"};

		QString answer = answer_val.toString();

		cards.emplace_back(id, question, answer);
	}

	return Deck(std::move(deckName), std::move(cards), {});
}

std::expected<Deck, QString> DeckUtils::deckAddStatsFromJson(Deck&& deck, const QJsonDocument& jsonDoc)
{
	QString prefix("Error parsing json: ");

	QJsonObject deckObj = jsonDoc.object();
	if(deckObj.isEmpty())
		return std::unexpected{prefix + "main component is not an object"};

	QJsonValue cardsArr_val = deckObj.value("cards");
	if(!cardsArr_val.isArray())
		return std::unexpected{prefix + "cards not found"};

	QJsonArray cardsArr = cardsArr_val.toArray();
    	for(const auto& card : std::as_const(cardsArr)){
		QJsonObject cardObj = card.toObject();
		if(cardObj.isEmpty())
			return std::unexpected{prefix + "card object is empty"};
		
		QJsonValue id_val = cardObj.value("id");
		if(!id_val.isDouble())
			return std::unexpected{prefix + "id is not a number"};

		int id = id_val.toInt();

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

		std::optional<std::reference_wrapper<Card>> res = deck.getCardById(id);
		if(!res) return std::unexpected{prefix + "card id not valid"};
		Card& c = res->get();

		c.setNextReviewDate(nextReviewDate);
		c.setEase(ease);
		c.setInterval(interval);
		c.setRepetitions(repetitions);
	}
	
	QJsonValue statsArr_val = deckObj.value("stats");
	if(!statsArr_val.isArray())
		return std::unexpected{prefix + "stats not found"};

	QJsonArray statsArr = statsArr_val.toArray();
	std::map<QDate, std::tuple<int, int, int>> master_history;
	
    	for(const auto& log : std::as_const(statsArr)){
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

	deck.setMasterHistory(std::move(master_history));
	return deck;
}

QJsonDocument DeckUtils::deckJsonFromDeck(const Deck& deck)
{
	QJsonObject mainObj;
	mainObj.insert("deck_name", deck.getName());
	
	QJsonArray cardsArray;
	for(const Card& card : deck.getCards()){
		QJsonObject cardObj;
		cardObj.insert("id", QJsonValue(card.getId()));
		cardObj.insert("question", QJsonValue(card.getQuestion()));
		cardObj.insert("answer", QJsonValue(card.getAnswer()));
		cardsArray.push_back(cardObj);
	}
	mainObj.insert("cards", cardsArray);

	QJsonDocument doc(mainObj);
	return doc;
}

QJsonDocument DeckUtils::statsJsonFromDeck(const Deck& deck)
{
	QJsonObject mainObj;
	
	QJsonArray cardsArray;
	for(const Card& card : deck.getCards()){
		QJsonObject cardObj;
		cardObj.insert("id", QJsonValue(card.getId()));
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
	str.remove(QRegularExpression("[/\\\\:.*?\"<>|]"));
	
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
            .arg(dirPath,
                 baseName)
            .arg(counter);

		if(!extension.isEmpty())
			str += "." + extension;

		counter++;
	}

	return str;
}

