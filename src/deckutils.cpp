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

	QVariantList ret;
	ret.reserve(fileList.size());
	for(const QFileInfo &fileInfo : fileList){
		QVariantMap map;
		map["file_path"] = std::move(fileInfo.absoluteFilePath());
		map["base_name"] = std::move(fileInfo.baseName());
		ret << map;
	}

	return ret;
}

DeckModel* DeckUtils::getDeckModel() const
{
	return m_deckModel.get();
}

void DeckUtils::loadDeck(const QString& fileName, DeckMode mode)
{
	Deck temp = deckFromJson(jsonFromFile(fileName));
	m_deckModel = std::make_unique<DeckModel>(std::move(temp), mode, this);
	emit deckModelChanged();
}

void DeckUtils::saveDeck(const QString& fileName)
{
	bool res = jsonToFile(jsonFromDeck(m_deckModel->getDeck()), fileName);

	if(!res)
		qDebug() << "Error saving deck to file";
}

void DeckUtils::deleteDeck(const QString& fileName)
{
	m_deckModel.reset();
	emit deckModelChanged();
	//QFile::rename(fileName, fileName+".deleted");
	QFile::remove(fileName);
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
		qDebug() << "Error saving deck to file";

	emit availableDecksChanged();
	return fileName;
}
	
QString DeckUtils::changeFileName(const QString& filePath, const QString& deckName)
{
	QFileInfo fileInfo(filePath);

	if(!fileInfo.exists())
		qWarning() << "File does not exist:" << filePath;

	QString sanitizedName = sanitizeFileName(deckName);
	QString newFilePath = QString("%1/%2.json")
				    .arg(fileInfo.absolutePath())
				    .arg(sanitizedName);

	if(filePath == newFilePath)
		return filePath;

	newFilePath = uniqueFileName(std::move(newFilePath));

	QFile file(filePath);
	if(!QFile::rename(filePath, newFilePath)){
		return filePath;
	}else{
		emit availableDecksChanged();
		return newFilePath; 
	}
}

QJsonDocument DeckUtils::jsonFromFile(const QString& jsonFilename) const
{
	//TODO use expected and FP
	QJsonDocument jsonDoc;

	QFile jsonFile(jsonFilename);
	if(!jsonFile.open(QIODevice::ReadOnly)){
		qDebug() << "Error:" << jsonFile.errorString();
		return jsonDoc;		
	}

	QJsonParseError jsonErr;
	jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &jsonErr);
	jsonFile.close();

	return jsonDoc;
}

bool DeckUtils::jsonToFile(const QJsonDocument& doc, const QString& fileName) const
{
	//TODO use expected and FP
	QString tempPath = fileName + ".tmp";
	QFile tempFile(tempPath);

	if(!tempFile.open(QIODevice::WriteOnly)){
		qDebug() << "Error: " << tempFile.errorString();
		return false;
	}

	tempFile.write(doc.toJson(QJsonDocument::Indented));
	tempFile.close();

	QFile::remove(fileName);
	return QFile::rename(tempPath, fileName);
}

Deck DeckUtils::deckFromJson(const QJsonDocument& jsonDoc) const
{	
	//TODO use expected and FP
	//fields are assumed to be there !!

	QJsonObject deckObj = jsonDoc.object();
	QString deckName = deckObj.find("deck_name").value().toString();

	QJsonArray cardsArr = deckObj.value("cards").toArray();
	std::vector<Card> cards;
	cards.reserve(cardsArr.size());
	
	for(auto card : cardsArr){
		QJsonObject cardObj = card.toObject();
		QString question = cardObj.find("question").value().toString();
		QString answer = cardObj.find("answer").value().toString();
		QDate creationDate = QDate::fromString(cardObj.find("date_creation").value().toString(), Qt::ISODate);
		QDate nextReview = QDate::fromString(cardObj.find("date_next_review").value().toString(), Qt::ISODate);
		double ease = cardObj.find("ease").value().toDouble();
		size_t interval = cardObj.find("interval").value().toInt();
		size_t repetition = cardObj.find("repetition").value().toInt();
		//TODO check values
		if(creationDate.isNull() || nextReview.isNull()){
			//DO SOMETHING!!
		}

		cards.emplace_back(question, answer, creationDate, nextReview, ease, interval, repetition);
	}

	QJsonArray statsArr = deckObj.value("stats").toArray();
	std::map<QDate, std::tuple<int, int, int>> master_history;
	
	for(auto log : statsArr){
		QJsonObject logObj = log.toObject();
		QDate logDate = QDate::fromString(logObj.find("date").value().toString(), Qt::ISODate);
		int n_new = logObj.find("new").value().toInt();
		int n_learning = logObj.find("learning").value().toInt();
		int n_mastered = logObj.find("mastered").value().toInt();
		
		//TODO check values
		if(logDate.isNull()){
			//DO SOMETHING!!
		}

		master_history[logDate] = std::make_tuple(n_new, n_learning, n_mastered);
	}
	
	return Deck(std::move(deckName), std::move(cards), master_history);
}

QJsonDocument DeckUtils::jsonFromDeck(const Deck& deck) const
{
	//TODO use expected and FP

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

