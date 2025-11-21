#pragma once

#include <QDate>
#include <QString>
#include "deckenums.h"

class Card{
public:
	Card(int id, QString question, QString answer);
	Card(int id, QString question, QString answer, QDate nextReviewDate, double ease, size_t interval, size_t repetitions);

	int getId() const;
	QString getQuestion() const;
	void setQuestion(QString text);
	QString getAnswer() const;
	void setAnswer(QString text);
	QDate getCreationDate() const;
	QDate getNextReviewDate() const;
	void setNextReviewDate(const QDate& new_date);
	double getEase() const;
	void setEase(double new_ease);
	size_t getInterval() const;
	void setInterval(size_t new_interval);
	size_t getRepetitions() const;
	void setRepetitions(size_t new_repetitions);

	bool toReview() const;
	void setResult(CardResult); 

private:
	int id;
	QString question = "";
	QString answer = "";
	QDate nextReviewDate = QDate::currentDate();
	double ease = 2.0;
	size_t interval = 1;
	size_t repetitions = 0;
};
