#pragma once

#include <vector>
#include <QString>
#include <ranges>
#include <span>
#include "card.h"

class Deck{
public:
	Deck();
	Deck(QString name);
	Deck(QString name, std::vector<Card>&& cards);

	QString getName() const;
	std::span<Card> getCards();
	std::span<const Card> getCards() const;
	void addCard(Card&& c);
	Card getCardAt(size_t idx) const;

	size_t size() const;

private:
	std::vector<Card> cards;
	QString name;
};
