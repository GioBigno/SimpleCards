#include "card.h"

Card::Card(int id, QString question, QString answer)
	: id(id),
	  question(std::move(question)), 
	  answer(std::move(answer))
{}

Card::Card(int id, QString question, QString answer, QDate nextReviewDate, double ease, size_t interval, size_t repetitions)
	: id(id),
	  question(std::move(question)), 
	  answer(std::move(answer)),
	  nextReviewDate(std::move(nextReviewDate)),
	  ease(ease),
	  interval(interval),
	  repetitions(repetitions)
{}

int Card::getId() const
{
	return id;
}

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

QDate Card::getNextReviewDate() const
{
	return nextReviewDate;
}

void Card::setNextReviewDate(const QDate& new_date)
{
	nextReviewDate = new_date;
}

double Card::getEase() const
{
	return ease;
}

void Card::setEase(double new_ease)
{
	ease = new_ease;
}

size_t Card::getInterval() const
{
	return interval;
}

void Card::setInterval(size_t new_interval)
{
	interval = new_interval;
}

size_t Card::getRepetitions() const
{
	return repetitions;
}

void Card::setRepetitions(size_t new_repetitions)
{
	repetitions = new_repetitions;
}

bool Card::toReview() const
{
	return nextReviewDate <= QDate::currentDate();
}

void Card::setResult(CardResult result)
{
	static constexpr double MIN_EASE = 1.3;
	static constexpr size_t MAX_INTERVAL = 365;

	switch(result){
		case CardResult::Wrong:
			repetitions = 0;
			interval = 1;
			ease = std::max(MIN_EASE, ease - 0.1);
			break;
		case CardResult::Hard:
			repetitions += 1;
			interval = std::round(interval * 1.2);
			interval = std::min(interval, MAX_INTERVAL);
			ease = std::max(MIN_EASE, ease - 0.05);
			break;
		case CardResult::Correct:
			repetitions += 1;
			interval = std::round(interval * ease);
			interval = std::min(interval, MAX_INTERVAL);
			ease += 0.03;
			break;
		default:
			std::unreachable();
	}

	nextReviewDate = QDate::currentDate().addDays(interval);
}
