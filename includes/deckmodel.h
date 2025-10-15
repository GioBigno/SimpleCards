#pragma once

#include <QAbstractListModel>
#include "deck.h"
#include "card.h"
#include "deckmode.h"
#include <QDebug>

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
	Deck getDeck() const;

	// Required overrides:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

	//these are needed when caller use this model outside of qml model views
	int size() const;
	Q_INVOKABLE QVariant getCardAt(size_t idx) const;
	Q_INVOKABLE void setResultAt(size_t idx, int result);
	Q_INVOKABLE void setQuestionAt(size_t idx, QString text);
	Q_INVOKABLE void setAnswerAt(size_t idx, QString text);
	Q_INVOKABLE void changeTitle(QString text);
	Q_INVOKABLE size_t addCard();
	Q_INVOKABLE void deleteCardAt(size_t idx);

private:
	Deck m_deck;
	size_t m_size = 0;
};
