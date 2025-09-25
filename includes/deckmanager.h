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

	Q_PROPERTY(QStringList availableDecksNames READ availableDecksNames NOTIFY availableDecksNamesChanged);
	Q_PROPERTY(QVariantMap currentCard READ currentCard NOTIFY currentCardChanged);
	Q_PROPERTY(QString currentDeckName READ currentDeckName NOTIFY currentDeckNameChanged);
	
public:
	Q_INVOKABLE bool loadDeck(size_t idx);
	//Q_INVOKABLE void setResult(some enum); (correct, wrong, skipped)
	Q_INVOKABLE bool goNext();

signals:
	void availableDecksNamesChanged();
	void currentCardChanged();
	void currentDeckNameChanged();

public: 
	explicit DeckManager(QObject *parent = nullptr);

	QStringList availableDecksNames() const;
	QVariantMap currentCard() const;
	QString currentDeckName() const;

private:
	std::vector<QString> decksFiles; // TODO questo full path
	std::optional<Deck> currentDeck;
	std::optional<size_t> m_currentCard;

	std::vector<QString> getDecksFiles() const;
	QJsonDocument getJsonDoc(const QString& jsonFilename) const;
	std::optional<Deck> getDeck(QJsonDocument jsonDoc) const;

	void debug_printDeck(Deck d) const;
};

