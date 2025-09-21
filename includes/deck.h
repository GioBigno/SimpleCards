#ifndef DECK_H
#define DECK_H

#include <vector>
#include <QString>
#include "card.h"

class Deck{
public:
	Deck(QString name);
	Deck(QString name, std::vector<Card>&& cards);

	QString getName() const;

	void addCard(Card&& c);
	Card getCardAt(size_t idx) const;

	size_t size();

private:
	std::vector<Card> cards;
	QString name;
};

#endif //DECK_H
