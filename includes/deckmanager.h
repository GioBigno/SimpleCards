#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QFile>
#include <QJsonDocument>
#include "deck.h"

#include <QtQml/qqmlregistration.h>

class DeckManager : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(QStringList availableDecks READ availableDecks NOTIFY availableDecksChanged);

public: 
    explicit DeckManager(QObject *parent = nullptr);

    QStringList availableDecks() const;

signals:
    void availableDecksChanged();

private:

	std::vector<std::unique_ptr<QFile>> decksFiles;
	QStringList m_availableDecks;

	std::vector<std::unique_ptr<QFile>> getDecksFiles() const;
	QJsonDocument getJsonDoc(QFile& jsonFile) const;
	Deck getDeck(QJsonDocument jsonDoc) const;

	void debug_printDeck(Deck d) const;
};

