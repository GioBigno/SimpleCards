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

std::span<Card> Deck::getCards()
{
	return std::span<Card>{cards};
}

std::span<const Card> Deck::getCards() const
{
	return std::span<const Card>{cards};
}

void Deck::addCard(Card&& c)
{
	cards.push_back(std::move(c));
}

Card Deck::getCardAt(size_t idx) const
{
	return cards.at(idx);
}

size_t Deck::size() const
{
	return cards.size();
}
