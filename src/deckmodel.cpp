#include "deckmodel.h"

DeckModel::DeckModel(Deck deck, DeckMode mode, QObject *parent) 
	: QAbstractListModel(parent),
	  m_deck(std::move(deck))
{
	if(mode == DeckMode::Test){
		m_cards = m_deck.getCards();
	}

	emit deckNameChanged();
}

QString DeckModel::getDeckName() const
{
	return m_deck.getName();
}

int DeckModel::rowCount(const QModelIndex &parent) const
{}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{}

QHash<int, QByteArray> DeckModel::roleNames() const
{}
