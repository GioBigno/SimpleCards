#include "card.h"
#include <cmath>
#include <algorithm>

Card::Card(QString question, QString answer)
	: question(std::move(question)), 
	  answer(std::move(answer)),
	  creationDate(QDate::currentDate()),
	  nextReview(QDate::currentDate())
{}

Card::Card(QString question, QString answer, QDate creationDate, QDate nextReview, double ease, size_t interval, size_t repetition)
	: question(std::move(question)), 
	  answer(std::move(answer)),
	  creationDate(std::move(creationDate)),
	  nextReview(std::move(nextReview)),
	  ease(ease),
	  interval(interval),
	  repetition(repetition)
{}

QString Card::getQuestion() const 
{
	return question;
}

QString Card::getAnswer() const 
{
	return answer;
}

bool Card::toReview() const
{
	return nextReview <= QDate::currentDate();
}

void Card::setResult(int result)
{
	static constexpr double MIN_EASE = 1.3;

	if(result == -1){
		repetition = 0;
		interval = 1;
		ease = std::max(MIN_EASE, ease - 0.2);
	}else if(result == 0){
		repetition += 1;
		interval = std::round(interval * 1.2);
		ease = std::max(MIN_EASE, ease - 0.05);
	}else{
		repetition += 1;
		interval = std::round(interval * ease);
		ease += 0.03;
	}

	nextReview = QDate::currentDate().addDays(interval);
}
