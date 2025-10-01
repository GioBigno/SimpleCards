#pragma once

#include <QDate>
#include <QString>

class Card{
public:
	Card(QString question, QString answer);
	Card(QString question, QString answer, QDate creationDate, QDate nextReview, double ease, size_t interval, size_t repetition);

	QString getQuestion() const;
	QString getAnswer() const;

	bool toReview() const;
	void setResult(int result); 

private:
	QString question;
	QString answer;
	QDate creationDate;
	QDate nextReview;
	double ease;
	size_t interval;
	size_t repetition;
};
