#include "deckmanager.h"
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

DeckManager::DeckManager(QObject *parent)
    : QObject{parent}
{
	decksFiles = std::move(getDecksFiles());
	emit availableDecksNamesChanged();
}

bool DeckManager::loadDeck(size_t idx)
{
	QJsonDocument jsonDoc = getJsonDoc(decksFiles.at(idx));
	
	if(jsonDoc.isNull()){
		qCritical() << "error parsing json";
		return false;
	}

	currentDeck = getDeck(jsonDoc);

	if(currentDeck.has_value() && currentDeck->size() > 0)
		m_currentCard = 0;
	else
		m_currentCard.reset();

	emit currentCardChanged();
	return true;
}

bool DeckManager::goNext()
{	
	if(!m_currentCard.has_value() || !currentDeck.has_value() || m_currentCard >= currentDeck->size()-1)
		return false;

	(*m_currentCard)++;
	emit currentCardChanged();
	return true;
}

QStringList DeckManager::availableDecksNames() const
{
	QStringList temp_availableDecksNames;
	temp_availableDecksNames.reserve(decksFiles.size());
	
	for(const auto& deckFile : decksFiles)
		temp_availableDecksNames.emplace_back(QFileInfo(deckFile).baseName());

	return temp_availableDecksNames;
}

QVariantMap DeckManager::currentCard() const
{
	QMap<QString, QVariant> temp_currentCard;

	if(!m_currentCard.has_value() || !currentDeck.has_value())
		return temp_currentCard;

	temp_currentCard.insert("q", currentDeck->getCardAt(*m_currentCard).getQuestion());
	temp_currentCard.insert("a", currentDeck->getCardAt(*m_currentCard).getAnswer());

	return temp_currentCard;
}

std::vector<QString> DeckManager::getDecksFiles() const
{	
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	QFileInfoList fileList = dataDir.entryInfoList(QStringList("*.json"), QDir::Files);

	std::vector<QString> ret;
	ret.reserve(fileList.size());

	//TODO use functional style std::transform
	for(const QFileInfo &fileInfo : fileList)
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

std::optional<Deck> DeckManager::getDeck(QJsonDocument jsonDoc) const
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
