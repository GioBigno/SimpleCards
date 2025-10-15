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
	void setName(QString text);
	std::span<Card> getCards();
	std::span<const Card> getCards() const;
	const Card& getCardAt(size_t idx) const;
	void addCard(Card&& c);
	void removeCardAt(size_t idx);
	void setQuestionAt(size_t idx, QString text);
	void setAnswerAt(size_t idx, QString text);
	void setResultAt(size_t idx, int result);

	size_t size() const;

private:
	std::vector<Card> cards;
	QString name;
};
