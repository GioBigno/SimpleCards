#include "deck.h"
#include "card.h"

Deck::Deck(QString name)
	: name(name)
{}

Deck::Deck(QString name, std::vector<Card>&& cards)
	: name(name), cards(std::move(cards))
{}

QString Deck::getName() const {
	return name;
}

void Deck::addCard(Card&& c){
	cards.push_back(std::move(c));
}

Card Deck::getCardAt(size_t idx) const {
	return cards.at(idx);
}

size_t Deck::size(){
	return cards.size();
}
