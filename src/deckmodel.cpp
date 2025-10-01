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

int DeckModel::size() const
{
	return m_cards.size();
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
	return size();
}

QVariant DeckModel::getCardAt(size_t idx) const
{
	if(idx >= m_cards.size())
        return {};

	Card& card = m_cards[idx];
	
	QVariantMap cardVariant;
	cardVariant["question"] = card.getQuestion();
	cardVariant["answer"]   = card.getAnswer();
	
	return cardVariant;
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
	qDebug() << "request for data: idx:" << index.row() << "role:" << role;
	
	if(!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_cards.size()))
		return {};
	
	Card& card = m_cards[index.row()];
	switch(role){
		case Roles::Question:
			return card.getQuestion();
			break;
		case Roles::Answer:
			return card.getAnswer();
			break;
		default:
			return {};
	}
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
	return {
		{Question, "question"},
		{Answer, "answer"}
	};
}
