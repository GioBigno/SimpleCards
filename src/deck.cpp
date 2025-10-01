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
/*
void Deck::shuffle()
{
	auto rd = std::random_device {};
	auto rng = std::default_random_engine {rd()};
	std::ranges::shuffle(cards, rng);
}
*/

std::span<Card> Deck::getCards()
{
	return std::span<Card>{cards};
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
