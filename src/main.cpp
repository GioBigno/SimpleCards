#include <iostream>
#include <string>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QtLogging>

#include "card.h"
#include "deck.h"

//Deck jsonToDeck(std::string deck_name)

bool checkDataDir();
std::vector<std::unique_ptr<QFile>> getDecksFiles();
QJsonDocument getJsonDoc(QFile& jsonFile);
Deck getDeck(QJsonDocument jsonDoc);

void debug_printDeck(Deck d);

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName("SimpleCards");
	QCoreApplication::setApplicationVersion("1.0");
	
	QGuiApplication app(argc, argv);
/*
	QQmlApplicationEngine engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
      	&app,
      	[]() { QCoreApplication::exit(-1); },
      	Qt::QueuedConnection);
	engine.loadFromModule("simplecards", "Main");
*/
	
	if(!checkDataDir()){
		qCritical() << "cannot use dataDir:" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		return -1;
	}

	std::vector<std::unique_ptr<QFile>> decksFiles = getDecksFiles();
	for(auto& deckFile : decksFiles){
		qDebug() << QFileInfo(deckFile->fileName()).baseName();
		Deck d = getDeck(getJsonDoc(*deckFile));
		debug_printDeck(d);
	}

	return app.exec();
}

bool checkDataDir()
{
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if (!dataDir.mkpath(dataDir.absolutePath())) {
		qDebug() << "Failed to create directory:" << dataDir;
		return false;
	}
	return true;
}

std::vector<std::unique_ptr<QFile>> getDecksFiles()
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

QJsonDocument getJsonDoc(QFile& jsonFile)
{
	QJsonDocument jsonDoc;

	if(!jsonFile.open(QIODevice::ReadOnly)){
		qDebug() << "Error:" << jsonFile.errorString();
		
	}

	QJsonParseError jsonErr;
	jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &jsonErr);
	jsonFile.close();
	if(jsonDoc.isNull()){
		qCritical() << "error parsing json";
		qCritical() << jsonErr.errorString();
	}

	return jsonDoc;
}

Deck getDeck(QJsonDocument jsonDoc)
{	
	//TODO if jsonDoc.isNull() or bad formatted throws exception

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

void debug_printDeck(Deck d)
{
	qDebug() << "name:" << d.getName();
	for(size_t i=0; i<d.size(); i++){
		Card c = d.getCardAt(i);
		qDebug() << "q:" << c.getQuestion() << "a:" << c.getAnswer();
	}
}
