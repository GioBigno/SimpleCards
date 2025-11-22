#include "appconfig.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QCoreApplication>

AppConfig::AppConfig(QObject *parent)
    : QObject{parent}
{}

QUrl AppConfig::decksDirectory_default(){
	return QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
}

bool AppConfig::getFirstOpening()
{
	return settings.value("utils/first_opening", firstOpening_default).toBool();
}

void AppConfig::setFirstOpening(bool opt)
{
	settings.setValue("utils/first_opening", opt);
	emit firstOpeningChanged();
}

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

bool AppConfig::getWindowMaximized()
{
	return settings.value("window/maximized", windowMaximized_default).toBool();
}

void AppConfig::setWindowMaximized(bool opt)
{
	settings.setValue("window/maximized", opt);
	emit windowMaximizedChanged();
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

QUrl AppConfig::getDecksDirectory()
{
	return settings.value("advanced/decks_directory", decksDirectory_default()).toUrl();
}

void AppConfig::setDecksDirectory(QUrl dir)
{
	settings.setValue("advanced/decks_directory", dir);
	emit decksDirectoryChanged();
}
