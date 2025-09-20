#include "deckmanager.h"
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>

DeckManager::DeckManager(QObject *parent)
    : QObject{parent}
{
	decksFiles = std::move(getDecksFiles());

	for(auto& deckFile : decksFiles)
		m_availableDecks.append(QFileInfo(deckFile->fileName()).baseName());
	emit availableDecksChanged();
}

QStringList DeckManager::availableDecks() const
{
	return m_availableDecks;	
}

std::vector<std::unique_ptr<QFile>> DeckManager::getDecksFiles() const
{	
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QStringList filters("*.json");
	QFileInfoList fileList = dataDir.entryInfoList(filters, QDir::Files);

	std::vector<std::unique_ptr<QFile>> ret;
	for (const QFileInfo &fileInfo : fileList) {
		auto jsonFile = std::make_unique<QFile>(fileInfo.absoluteFilePath());
		ret.push_back(std::move(jsonFile));
	}

	return ret;
}

QJsonDocument DeckManager::getJsonDoc(QFile& jsonFile) const
{
	QJsonDocument jsonDoc;

	if(!jsonFile.open(QIODevice::ReadOnly)){
		qDebug() << "Error:" << jsonFile.errorString();
		return jsonDoc;		
	}

	QJsonParseError jsonErr;
	jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &jsonErr);
	jsonFile.close();

	return jsonDoc;
}

Deck DeckManager::getDeck(QJsonDocument jsonDoc) const
{	
	//TODO if jsonDoc.isNull() or bad formatted throw exception

	QJsonObject deckObj = jsonDoc.object();
	std::string deckName = deckObj.find("deck_name").value().toString().toStdString();

	QJsonArray cardsArr = deckObj.value("cards").toArray();
	std::vector<Card> cards;
	cards.reserve(cardsArr.size());
	
	for(auto card : cardsArr){
		QJsonObject cardObj = card.toObject();
		std::string cardQuestion = cardObj.find("question").value().toString().toStdString();
		std::string cardAnswer = cardObj.find("answer").value().toString().toStdString();

		cards.emplace_back(cardQuestion, cardAnswer);
	}

	return Deck(deckName, std::move(cards));
}


void DeckManager::debug_printDeck(Deck d) const
{
	qDebug() << "name:" << d.getName();
	for(size_t i=0; i<d.size(); i++){
		Card c = d.getCardAt(i);
		qDebug() << "q:" << c.getQuestion() << "a:" << c.getAnswer();
	}
}
