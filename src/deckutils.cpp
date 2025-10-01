#include "deckutils.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

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

Deck DeckUtils::deckFromJson(const QJsonDocument& jsonDoc) const
{	
	//TODO use expected and FP

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

	return Deck(std::move(deckName), std::move(cards));
}
