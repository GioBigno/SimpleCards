#pragma once

#include <vector>
#include <map>
#include <span>
#include <tuple>
#include <optional>
#include "card.h"

class Deck{
public:
	Deck();
	Deck(QString name);
	//TODO replace vector with span
	Deck(QString name, std::vector<Card>&& cards, std::map<QDate, std::tuple<int, int, int>>&& master_history);

	QString getName() const;
	void setName(QString text);
	std::span<Card> getCards();
	std::span<const Card> getCards() const;
	const Card& getCardAt(size_t idx) const;
	std::optional<std::reference_wrapper<Card>> getCardById(int id);
	void addNewCard();
	void removeCardAt(size_t idx);
	void setQuestionAt(size_t idx, QString text);
	void setAnswerAt(size_t idx, QString text);
	void setResultAt(size_t idx, CardResult result);
	std::map<QDate, std::tuple<int, int, int>> getMasterHistory() const;
	void setMasterHistory(std::map<QDate, std::tuple<int, int, int>>&& new_master_history);

	size_t size() const;
	
	enum CardMasteryLevel{
		New,
		Learning,
		Mastered
	};

private:
	std::vector<Card> cards;
	std::map<QDate, std::tuple<int, int, int>> master_history;
	QString name;

	CardMasteryLevel calculateMastery(const Card& card) const;
	void increaseTupleByMastery(std::tuple<int, int, int>& t, CardMasteryLevel mastery);
	void decreaseTupleByMastery(std::tuple<int, int, int>& t, CardMasteryLevel mastery);
	void updateStats();
	int nextValidId() const;
};
