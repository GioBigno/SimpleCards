#include "deckmodel.h"
#include <random>
#include <algorithm>
#include <ranges>

DeckModel::DeckModel(Deck deck, DeckMode mode, QObject *parent) 
	: QAbstractListModel(parent),
	  m_deck(std::move(deck))
{
	if(mode == DeckMode::Test){
		std::span<Card> temp = m_deck.getCards();
		
		//move all cards to review to the front
		auto subrange = std::ranges::partition(temp, [](Card c){return c.toReview();});
		m_size = std::ranges::distance(temp.cbegin(), subrange.cbegin());

		//shuffle m_cards
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::shuffle(temp.begin(), subrange.begin(), gen);
	}else{
		m_size = m_deck.size();
	}
}

QString DeckModel::getDeckName() const
{
	return m_deck.getName();
}

Deck DeckModel::getDeck() const
{
	return m_deck;
}

QVariantList DeckModel::getStatsHistory() const
{
	QVariantList dataList;
	for(auto const& [date, level] : m_deck.getMasterHistory()){
		QVariantMap entry;
		entry["date"] = date;
      	entry["new"] = std::get<0>(level);
      	entry["learning"] = std::get<1>(level);
		entry["mastered"] = std::get<2>(level);
		dataList.append(entry);
	}

	return dataList;
}

int DeckModel::size() const
{
	return m_size;
}

int DeckModel::rowCount(const QModelIndex &parent) const
{
	return size();
}

QVariant DeckModel::getCardAt(size_t idx) const
{
	if(idx >= m_size)
        return {};

	const Card& card = m_deck.getCardAt(idx);
	
	QVariantMap cardVariant;
	cardVariant["question"] = card.getQuestion();
	cardVariant["answer"]   = card.getAnswer();
	cardVariant["mastery"]   = card.getEase();
	
	return cardVariant;
}

QVariant DeckModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_size))
		return {};
	
	const Card& card = m_deck.getCardAt(index.row());
	switch(role){
		case Roles::Question:
			return card.getQuestion();
			break;
		case Roles::Answer:
			return card.getAnswer();
			break;
		case Roles::Mastery:
			return card.getEase();
			break;
		default:
			return {};
	}
}

void DeckModel::setResultAt(size_t idx, CardResult result)
{
	m_deck.setResultAt(idx, result);
	emit statsHistoryChanged();
	emit dataChanged(this->index(static_cast<int>(idx)), this->index(static_cast<int>(idx)), {Mastery});
}

QHash<int, QByteArray> DeckModel::roleNames() const
{
	return {
		{Question, "question"},
		{Answer, "answer"},
		{Mastery, "mastery"}
	};
}

void DeckModel::setQuestionAt(size_t idx, QString text)
{
	if(idx >= m_size)
		return;

	m_deck.setQuestionAt(idx, text);
	emit dataChanged(this->index(static_cast<int>(idx)), this->index(static_cast<int>(idx)), {Question});
}

void DeckModel::setAnswerAt(size_t idx, QString text)
{
	if(idx >= m_size)
		return;

	m_deck.setAnswerAt(idx, text);
	emit dataChanged(this->index(static_cast<int>(idx)), this->index(static_cast<int>(idx)), {Answer});
}

void DeckModel::changeTitle(QString text)
{
	m_deck.setName(std::move(text));
	emit deckNameChanged();
}

size_t DeckModel::addCard()
{
	beginInsertRows(QModelIndex(), m_size, m_size);
	m_deck.addNewCard();
	m_size = m_deck.size();
	endInsertRows();
	emit sizeChanged();
	emit statsHistoryChanged();
	return m_size - 1;
}

void DeckModel::deleteCardAt(size_t idx)
{
	if(idx >= m_size)
		return;

	beginRemoveRows(QModelIndex(), static_cast<int>(idx), static_cast<int>(idx));
	m_deck.removeCardAt(idx);
	endRemoveRows();
	m_size = m_deck.size();
	emit sizeChanged();
	emit statsHistoryChanged();
}
