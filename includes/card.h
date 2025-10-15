#pragma once

#include <QDate>
#include <QString>

class Card{
public:
	Card();
	Card(QString question, QString answer);
	Card(QString question, QString answer, QDate creationDate, QDate nextReviewDate, double ease, size_t interval, size_t repetitions);

	QString getQuestion() const;
	void setQuestion(QString text);
	QString getAnswer() const;
	void setAnswer(QString text);
	QDate getCreationDate() const;
	QDate getNextReviewDate() const;
	double getEase() const;
	size_t getInterval() const;
	size_t getRepetitions() const;

	bool toReview() const;
	void setResult(int result); 

private:
	QString question = "";
	QString answer = "";
	QDate creationDate = QDate::currentDate();
	QDate nextReviewDate = QDate::currentDate();
	double ease = 2.0;
	size_t interval = 1;
	size_t repetitions = 0;
};
