#include "card.h"
#include <format>
#include <QDebug>

Card::Card(QString question, QString answer)
	: question(std::move(question)), 
	  answer(std::move(answer)),
	  creationDate(QDate::currentDate()),
	  lastReview(QDate::currentDate()),
	  nextReview(QDate::currentDate())
{}

Card::Card(QString question, QString answer, QDate creationDate, QDate lastReview, QDate nextReview)
	: question(std::move(question)), 
	  answer(std::move(answer)),
	  creationDate(std::move(creationDate)),
	  lastReview(std::move(lastReview)),
	  nextReview(std::move(nextReview))
{}

QString Card::getQuestion() const 
{
	return question;
}

QString Card::getAnswer() const 
{
	return answer;
}

QDate Card::getLastReview() const
{
	return lastReview;
}

QDate Card::getNextReview() const
{
	return nextReview;
}

bool Card::toReview() const
{
	return lastReview < nextReview;
}
