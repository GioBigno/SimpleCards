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

#if defined Q_OS_WASM
	#include <emscripten.h>
	void loadFS_and_run_app(const QDir& dir);
#endif

bool checkDataDir(QDir dir);

QQmlApplicationEngine* engine_ptr = nullptr;

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
	engine_ptr = &engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
      	&app,
      	[]() { QCoreApplication::exit(-1); },
      	Qt::QueuedConnection);


#if defined Q_OS_WASM
	loadFS_and_run_app(dataDir);
#else
	engine.loadFromModule("simplecardsModule", "Main");
#endif

	return app.exec();
}

#if defined Q_OS_WASM
void loadFS_and_run_app(const QDir& dir)
{
	const char* pathCStr = dir.absolutePath().toUtf8().constData();

	EM_ASM_({
		var path = UTF8ToString($0);
		console.log("Mounting IDBFS at: " + path);
		FS.mkdirTree(path);

		// Mount the IDBFS (IndexedDB) to this specific folder
		FS.mount(IDBFS, {}, path);

		// Pull data from Database -> RAM
		FS.syncfs(true, function (err) {
			if(err)
				console.log("Error loading DB: " + err);
			else
				console.log("FileSystem loaded successfully at " + path);
			_fileSystemLoaded();
		});
	}, pathCStr);
}

extern "C" {
	EMSCRIPTEN_KEEPALIVE
	void fileSystemLoaded(){
		qDebug() << "running app";
		engine_ptr->loadFromModule("simplecardsModule", "Main");
	}
}
#endif

bool checkDataDir(QDir dir)
{
	if(!dir.mkpath(dir.absolutePath())){
		qCritical() << "[main] Failed to create directory:" << dir;
		return false;
	}
	return true;
}

