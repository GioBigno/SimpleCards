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

	m_availableDecks.clear();
	m_availableDecks.reserve(decksFiles.size());

	//TODO use functional style std::transform
	for(const auto& deckFile : decksFiles)
		m_availableDecks.append(QFileInfo(deckFile).baseName());
	emit availableDecksChanged();
}

QStringList DeckManager::availableDecks() const
{
	return m_availableDecks;	
}

std::vector<QString> DeckManager::getDecksFiles() const
{	
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QFileInfoList fileList = dataDir.entryInfoList(QStringList("*.json"), QDir::Files);

	std::vector<QString> ret;
	ret.reserve(fileList.size());

	//TODO use functional style std::transform
	for (const QFileInfo &fileInfo : fileList)
		ret.emplace_back(fileInfo.absoluteFilePath());

	return ret;
}

QJsonDocument DeckManager::getJsonDoc(const QString& jsonFilename) const
{
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

Deck DeckManager::getDeck(QJsonDocument jsonDoc) const
{	
	//TODO if jsonDoc.isNull() or bad formatted throw exception

	QJsonObject deckObj = jsonDoc.object();
	QString deckName = deckObj.find("deck_name").value().toString();

	QJsonArray cardsArr = deckObj.value("cards").toArray();
	std::vector<Card> cards;
	cards.reserve(cardsArr.size());
	
	for(auto card : cardsArr){
		QJsonObject cardObj = card.toObject();
		QString cardQuestion = cardObj.find("question").value().toString();
		QString cardAnswer = cardObj.find("answer").value().toString();

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
