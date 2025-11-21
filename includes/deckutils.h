#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QtQml/qqmlregistration.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringView>
#include <expected>
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
	Q_INVOKABLE QString loadDeck(const QString& deckFileName, DeckMode mode);
	Q_INVOKABLE void saveLoadedDeck();
	Q_INVOKABLE void deleteLoadedDeck();
	Q_INVOKABLE void deleteDeck(const QString& deckFilePath);
	Q_INVOKABLE QString createEmptyDeckFile();
	Q_INVOKABLE void changeLoadedDeckFileName(const QString& deckName);

	explicit DeckUtils(QObject *parent = nullptr);

private:
	QString m_deckFilePath = "";
	std::unique_ptr<DeckModel> m_deckModel;
	
	static constexpr QStringView DECK_FILE_SUFFIX = u".deck.json";
	static constexpr QStringView STATS_FILE_SUFFIX = u".user.json";

	static QString statsFileNameFromDeckFileName(const QString& deckFileName);
	static std::expected<QJsonDocument, QString> jsonFromFile(const QString& jsonFilename);
	static bool jsonToFile(const QJsonDocument& jsonDoc, const QString& fileName);
	static std::expected<Deck, QString> deckFromJson(const QJsonDocument& jsonDoc);
	static std::expected<Deck, QString> deckAddStatsFromJson(Deck&& d, const QJsonDocument& jsonDoc);
	static QJsonDocument deckJsonFromDeck(const Deck& deck);
	static QJsonDocument statsJsonFromDeck(const Deck& deck);
	QString sanitizeFileName(QString str) const;
	QString uniqueFileName(QString str) const;
};
