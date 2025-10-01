#pragma once

#include <QAbstractListModel>
#include "deck.h"
#include "card.h"
#include "deckmode.h"

class DeckModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(QString deckName READ getDeckName NOTIFY deckNameChanged);

signals:
	void deckNameChanged();

public:
	enum Roles {
		QuestionRole = Qt::UserRole + 1,
		AnswerRole
	};
	Q_ENUM(Roles)

	explicit DeckModel(Deck deck, DeckMode mode, QObject *parent = nullptr);

	QString getDeckName() const;

	// Required overrides:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

private:
	Deck m_deck;
	std::span<Card> m_cards;
};
