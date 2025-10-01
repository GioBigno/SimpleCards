#pragma once

#include <QAbstractListModel>
#include "deck.h"
#include "card.h"
#include "deckmode.h"

class DeckModel : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(QString deckName READ getDeckName NOTIFY deckNameChanged);
	Q_PROPERTY(int size READ size NOTIFY sizeChanged);

signals:
	void deckNameChanged();
	void sizeChanged();

public:
	enum Roles{
		Question = Qt::UserRole + 1,
		Answer
	};
	Q_ENUM(Roles)

	explicit DeckModel(Deck deck, DeckMode mode, QObject *parent = nullptr);

	QString getDeckName() const;

	// Required overrides:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

	//these are needed when caller use this model outside of qml model views
	int size() const;
	Q_INVOKABLE QVariant getCardAt(size_t idx) const;
	Q_INVOKABLE void setResultAt(size_t idx, int result);

private:
	Deck m_deck;
	std::span<Card> m_cards;
};
