#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QFontDatabase>
#include <QtQml/qqmlregistration.h>
#include "card.h"
#include "deck.h"
#include "appconfig.h"

bool checkDataDir(QDir dir);

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("BignoSoft");
	QCoreApplication::setApplicationName("SimpleCards");
	QCoreApplication::setApplicationVersion("1.0");	
	QGuiApplication app(argc, argv);

	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Regular.ttf");
	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Bold.ttf");
	QFontDatabase::addApplicationFont(":/qt/qml/simplecardsModule/assets/SpaceMono-Italic.ttf");
	
	AppConfig conf;
	QDir dataDir = conf.getDecksDirectory().toLocalFile();

	if(!checkDataDir(dataDir)){
		qCritical() << "[main] cannot use dataDir:" << dataDir;
		return -1;
	}
	
	if(conf.getFirstOpening()){
		conf.setFirstOpening(false);
		QFile::copy(":qt/qml/simplecardsModule/example_decks/Example.deck.json", dataDir.absolutePath()+"/Example.deck.json");
	}

	QQmlApplicationEngine engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
      	&app,
      	[]() { QCoreApplication::exit(-1); },
      	Qt::QueuedConnection);
	engine.loadFromModule("simplecardsModule", "Main");
	return app.exec();
}

bool checkDataDir(QDir dir)
{
	if(!dir.mkpath(dir.absolutePath())){
		qCritical() << "[main] Failed to create directory:" << dir;
		return false;
	}
	return true;
}

