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
#include <QFontDatabase>
#include <QtQml/qqmlregistration.h>

#include "card.h"
#include "deck.h"
//#include "deckmanager.h"

bool checkDataDir();

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("BignoSoft");
	QCoreApplication::setApplicationName("SimpleCards");
	QCoreApplication::setApplicationVersion("1.0");	
	QGuiApplication app(argc, argv);

	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Regular.ttf");
	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Bold.ttf");
	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Italic.ttf");

	QQmlApplicationEngine engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
      	&app,
      	[]() { QCoreApplication::exit(-1); },
      	Qt::QueuedConnection);
	engine.loadFromModule("simplecardsModule", "Main");
	
	if(!checkDataDir()){
		qCritical() << "cannot use dataDir:" << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		return -1;
	}
	

/*
	std::vector<std::unique_ptr<QFile>> decksFiles = getDecksFiles();
	for(auto& deckFile : decksFiles){
		qDebug() << QFileInfo(deckFile->fileName()).baseName();

		JSonDocument jsonDoc = getJsonDoc(*deckFile);
		if(jsonDoc.isNull()){
			qCritical() << "error parsing json";
			qCritical() << jsonErr.errorString();
		}

		Deck d = getDeck(jsonDoc);
		debug_printDeck(d);
	}
*/
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

