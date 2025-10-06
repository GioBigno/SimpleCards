#include "deckmodel.h"
#include <random>
#include <algorithm>

DeckModel::DeckModel(Deck deck, DeckMode mode, QObject *parent) 
	: QAbstractListModel(parent),
	  m_deck(std::move(deck))
{
	if(mode == DeckMode::Test){
		m_cards = m_deck.getCards();
		
		//move all cards to review to the front
		auto subrange = std::ranges::partition(m_cards, [](Card c){return c.toReview();});

		//resize m_cards
		size_t n_to_review = std::ranges::distance(m_cards.cbegin(), subrange.cbegin());
		m_cards = m_cards.subspan(0, n_to_review);

		//shuffle m_cards
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::ranges::shuffle(m_cards, gen);
	}else{
		//order by creation date		
	}

	emit deckNameChanged();
	emit sizeChanged();
}

QString DeckModel::getDeckName() const
{
	return m_deck.getName();
}

Deck DeckModel::getDeck() const
{
	return m_deck;
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

void DeckModel::setResultAt(size_t idx, int result)
{
	//-1 wrong, 0 hard/almost, 1 correct
	m_cards[idx].setResult(result);
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
	return {
		{Question, "question"},
		{Answer, "answer"}
	};
}
