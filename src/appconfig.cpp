#include "appconfig.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>

AppConfig::AppConfig(QObject *parent)
    : QObject{parent}
{}

bool AppConfig::getHardButton()
{
	return settings.value("testing/hard_button", true).toBool();
}

void AppConfig::setHardButton(bool opt)
{

	settings.setValue("testing/hard_button", opt);
	emit hardButtonChanged();
}

bool AppConfig::getCardsCounter()
{
	return settings.value("testing/cards_counter", true).toBool();
}

void AppConfig::setCardsCounter(bool opt)
{
	settings.setValue("testing/cards_counter", opt);
	emit cardsCounterChanged();
}

bool AppConfig::getRememberSize()
{
	return settings.value("window/remember_size", false).toBool();
}

void AppConfig::setRememberSize(bool opt)
{
	settings.setValue("window/remember_size", opt);
	emit rememberSizeChanged();
}

QSize AppConfig::getWindowSize()
{
	return settings.value("window/size", QSize(800, 550)).toSize();
}

void AppConfig::setWindowSize(const QSize& s)
{
	settings.setValue("window/size", s);
	emit windowSizeChanged();
}
