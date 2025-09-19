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

std::unique_ptr<QFile> openDataFile(QString fileName);
Deck getDeck(QJsonDocument jsonDoc);

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
	
	std::unique_ptr<QFile> jsonFile = openDataFile("security.json");
	if(!jsonFile){
		qCritical() << "impossible to open data file";
		return -1;
	}

	QJsonParseError jsonErr;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile->readAll(), &jsonErr);
	jsonFile->close();
	if(jsonDoc.isNull()){
		qCritical() << "error parsing json";
		qCritical() << jsonErr.errorString();
		return -1;
	}

	getDeck(jsonDoc);

	return app.exec();
}

std::unique_ptr<QFile> openDataFile(QString fileName)
{
	QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
	if (!dataDir.mkpath(dataDir.absolutePath())) {
		qDebug() << "Failed to create directory:" << dataDir;
		return nullptr;
	}
	
	QString jsonFilePath = dataDir.absoluteFilePath(fileName);
	auto jsonFile = std::make_unique<QFile>(jsonFilePath);
	
	if(!jsonFile->open(QIODevice::ReadWrite)){
		qDebug() << "error opening data file, path:" << jsonFilePath;
		qDebug() << "Error:" << jsonFile->errorString();
		return nullptr;
	}else{
		qDebug() << "ok";
	}

	return jsonFile;
}

Deck getDeck(QJsonDocument jsonDoc){
	
	QJsonObject deckObj = jsonDoc.object();
		
	if(!deckObj.contains("deck_name")){
		//throw badJson exception
		qDebug() << "no name";
	}
		
	QJsonValue v = deckObj.find("deck_name").value();

	if(!v.isString()){
		qDebug() << "not a string";
	}

	std::string name = v.toString().toStdString();

	qDebug() << "all ok";

	return Deck(name);
}
