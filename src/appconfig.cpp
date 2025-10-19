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
