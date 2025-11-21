#include "deck.h"
#include "card.h"
#include <ranges>

Deck::Deck(){}

Deck::Deck(QString name)
	: name(std::move(name))
{}

Deck::Deck(QString name, std::vector<Card>&& cards, std::map<QDate, std::tuple<int, int, int>>&& master_history)
	: name(std::move(name)),
	  cards(std::move(cards)),
	  master_history(std::move(master_history))
{
	updateStats();
}

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

std::optional<std::reference_wrapper<Card>> Deck::getCardById(int id)
{
	auto it = std::ranges::find_if(cards, [id](Card& c){return c.getId() == id;});

	if(it == cards.end())
		return std::nullopt;
	return *it;
}

void Deck::addNewCard()
{
	Card c(nextValidId(), "", "");

	auto& today_tuple = master_history[QDate::currentDate()];
	CardMasteryLevel mastery = calculateMastery(c);
	increaseTupleByMastery(today_tuple, mastery);

	cards.push_back(std::move(c));
}

size_t Deck::size() const
{
	return cards.size();
}

void Deck::removeCardAt(size_t idx)
{
	auto& today_tuple = master_history[QDate::currentDate()];
	CardMasteryLevel mastery = calculateMastery(cards[idx]);
	decreaseTupleByMastery(today_tuple, mastery);
	
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

void Deck::setResultAt(size_t idx, CardResult result)
{
	auto& today_tuple = master_history[QDate::currentDate()];
	CardMasteryLevel old_mastery = calculateMastery(cards[idx]);
	decreaseTupleByMastery(today_tuple, old_mastery);

	cards[idx].setResult(result);

	CardMasteryLevel new_mastery = calculateMastery(cards[idx]);
	increaseTupleByMastery(today_tuple, new_mastery);
}

std::map<QDate, std::tuple<int, int, int>> Deck::getMasterHistory() const
{
	return master_history;
}

void Deck::setMasterHistory(std::map<QDate, std::tuple<int, int, int>>&& new_master_history)
{
	master_history = std::move(new_master_history);
	updateStats();
}

Deck::CardMasteryLevel Deck::calculateMastery(const Card& c) const
{
	const double ease = c.getEase();
	const size_t repetitions = c.getRepetitions();

	if(ease < 2.05 && repetitions < 2)
		return CardMasteryLevel::New;
	if(ease < 2.1 || repetitions < 6)
		return CardMasteryLevel::Learning;
	return CardMasteryLevel::Mastered;
}

void Deck::increaseTupleByMastery(std::tuple<int, int, int>& t, CardMasteryLevel mastery)
{
	switch(mastery){
		case CardMasteryLevel::New: std::get<0>(t)++; break;
		case CardMasteryLevel::Learning: std::get<1>(t)++; break;
		case CardMasteryLevel::Mastered: std::get<2>(t)++; break;
		default: std::unreachable();
	}
}

void Deck::decreaseTupleByMastery(std::tuple<int, int, int>& t, CardMasteryLevel mastery)
{
	switch(mastery){
		case CardMasteryLevel::New: std::get<0>(t)--; break;
		case CardMasteryLevel::Learning: std::get<1>(t)--; break;
		case CardMasteryLevel::Mastered: std::get<2>(t)--; break;
		default: std::unreachable();
	}
}

void Deck::updateStats()
{
	auto& today_tuple = master_history[QDate::currentDate()];
	today_tuple = std::make_tuple(0, 0, 0);

	for(const Card& c : std::as_const(cards)){
		increaseTupleByMastery(today_tuple, calculateMastery(c));
	}
}

int Deck::nextValidId() const
{
	if(cards.size() == 0)
		return 0;

	int maxId = cards[0].getId();
	for(const Card& c : std::as_const(cards))
		maxId = std::max(maxId, c.getId());

	return maxId+1;
}
