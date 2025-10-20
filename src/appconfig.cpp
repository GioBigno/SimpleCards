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
	return settings.value("testing/hard_button", hardButton_default).toBool();
}

void AppConfig::setHardButton(bool opt)
{
	settings.setValue("testing/hard_button", opt);
	emit hardButtonChanged();
}

bool AppConfig::getCardsCounter()
{
	return settings.value("testing/cards_counter", cardsCounter_default).toBool();
}

void AppConfig::setCardsCounter(bool opt)
{
	settings.setValue("testing/cards_counter", opt);
	emit cardsCounterChanged();
}

bool AppConfig::getRememberSize()
{
	return settings.value("window/remember_size", rememberSize_default).toBool();
}

void AppConfig::setRememberSize(bool opt)
{
	settings.setValue("window/remember_size", opt);
	emit rememberSizeChanged();
}

QSize AppConfig::getWindowSize()
{
	return settings.value("window/size", windowSize_default).toSize();
}

void AppConfig::setWindowSize(const QSize& s)
{
	settings.setValue("window/size", s);
	emit windowSizeChanged();
}

bool AppConfig::getConfirmDeleteDeck()
{
	return settings.value("editing/confirm_delete_deck", confirmDeleteDeck_default).toBool();
}

void AppConfig::setConfirmDeleteDeck(bool opt)
{
	settings.setValue("editing/confirm_delete_deck", opt);
	emit confirmDeleteDeckChanged();
}

bool AppConfig::getConfirmDeleteCard()
{
	return settings.value("editing/confirm_delete_card", confirmDeleteCard_default).toBool();
}

void AppConfig::setConfirmDeleteCard(bool opt)
{
	settings.setValue("editing/confirm_delete_card", opt);
	emit confirmDeleteCardChanged();
}
