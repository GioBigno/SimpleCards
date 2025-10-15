#include "deck.h"
#include "card.h"
#include <algorithm>
#include <random>
#include <ranges>
#include <QDebug>

Deck::Deck(){}

Deck::Deck(QString name)
	: name(std::move(name))
{}

Deck::Deck(QString name, std::vector<Card>&& cards)
	: name(std::move(name)),
	  cards(std::move(cards))
{}

QString Deck::getName() const
{
	return name;
}

void Deck::setName(QString text)
{
	name = std::move(text);
}

std::span<Card> Deck::getCards()
{
	return std::span<Card>{cards};
}

std::span<const Card> Deck::getCards() const
{
	return std::span<const Card>{cards};
}

const Card& Deck::getCardAt(size_t idx) const
{
	return cards[idx];
}

void Deck::addCard(Card&& c)
{
	cards.push_back(std::move(c));
}

size_t Deck::size() const
{
	return cards.size();
}

void Deck::removeCardAt(size_t idx)
{
	cards.erase(cards.begin() + idx);
}

void Deck::setQuestionAt(size_t idx, QString text)
{
	cards[idx].setQuestion(text);	
}

void Deck::setAnswerAt(size_t idx, QString text)
{
	cards[idx].setAnswer(text);
}

void Deck::setResultAt(size_t idx, int result)
{
	cards[idx].setResult(result);
}
}
