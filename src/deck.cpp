#include "deck.h"
#include "card.h"

Deck::Deck(std::string name)
	: name(name)
{}

Deck::Deck(std::string name, std::vector<Card>&& cards)
	: name(name), cards(std::move(cards))
{}

std::string Deck::getName() const {
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
