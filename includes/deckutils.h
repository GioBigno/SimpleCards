#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QtQml/qqmlregistration.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "deck.h"
#include "deckmodel.h"
#include "deckenums.h"

class DeckUtils : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON

	Q_PROPERTY(QVariantList availableDecks READ getAvailableDecks NOTIFY availableDecksChanged)
	Q_PROPERTY(DeckModel* deckModel READ getDeckModel NOTIFY deckModelChanged)

signals:
	void availableDecksChanged();
	void deckModelChanged();

public:
	QVariantList getAvailableDecks() const;
	DeckModel* getDeckModel() const;
	Q_INVOKABLE void loadDeck(const QString& fileName, DeckMode mode);
	Q_INVOKABLE void saveDeck(const QString& fileName);
	Q_INVOKABLE void deleteDeck(const QString& fileName);
	Q_INVOKABLE QString createEmptyDeckFile();
	Q_INVOKABLE QString changeFileName(const QString& filePath, const QString& deckName);

	explicit DeckUtils(QObject *parent = nullptr);

private:
	QString m_deckFilePath;
	std::unique_ptr<DeckModel> m_deckModel;
	
	QJsonDocument jsonFromFile(const QString& jsonFilename) const; //exception??
	bool jsonToFile(const QJsonDocument& jsonDoc, const QString& fileName) const; //exception??
	Deck deckFromJson(const QJsonDocument& jsonDoc) const; //exception?
	QJsonDocument jsonFromDeck(const Deck& deck) const; //exception??
	QString sanitizeFileName(QString str) const;
	QString uniqueFileName(QString str) const;
};
