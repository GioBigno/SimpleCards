#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

class Deck{
public:
	Deck(std::string name);
	Deck(std::string name, std::vector<Card>&& cards);

	std::string getName() const;

	void addCard(Card&& c);
	Card getCardAt(size_t idx) const;

	size_t size();

private:
	std::vector<Card> cards;
	std::string name;
};

#endif //DECK_H
