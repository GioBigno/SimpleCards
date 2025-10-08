#include "card.h"
#include <cmath>
#include <algorithm>

Card::Card(QString question, QString answer)
	: question(std::move(question)), 
	  answer(std::move(answer))
{}

Card::Card(QString question, QString answer, QDate creationDate, QDate nextReviewDate, double ease, size_t interval, size_t repetitions)
	: question(std::move(question)), 
	  answer(std::move(answer)),
	  creationDate(std::move(creationDate)),
	  nextReviewDate(std::move(nextReviewDate)),
	  ease(ease),
	  interval(interval),
	  repetitions(repetitions)
{}

QString Card::getQuestion() const 
{
	return question;
}

void Card::setQuestion(QString text)
{
	question = std::move(text);
}

QString Card::getAnswer() const 
{
	return answer;
}

void Card::setAnswer(QString text)
{
	answer = std::move(text);
}
	
QDate Card::getCreationDate() const
{
	return creationDate;
}

QDate Card::getNextReviewDate() const
{
	return nextReviewDate;
}

double Card::getEase() const
{
	return ease;
}

size_t Card::getInterval() const
{
	return interval;
}

size_t Card::getRepetitions() const
{
	return repetitions;
}

bool Card::toReview() const
{
	return nextReviewDate <= QDate::currentDate();
}

void Card::setResult(int result)
{
	static constexpr double MIN_EASE = 1.3;

	if(result == -1){
		repetitions = 0;
		interval = 1;
		ease = std::max(MIN_EASE, ease - 0.2);
	}else if(result == 0){
		repetitions += 1;
		interval = std::round(interval * 1.2);
		ease = std::max(MIN_EASE, ease - 0.05);
	}else{
		repetitions += 1;
		interval = std::round(interval * ease);
		ease += 0.03;
	}

	nextReviewDate = QDate::currentDate().addDays(interval);
}
