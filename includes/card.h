#pragma once

#include <QDate>
#include <QString>

class Card{
public:
	Card(QString question, QString answer);
	Card(QString question, QString answer, QDate creationDate, QDate lastReview, QDate nextReview);

	QString getQuestion() const;
	QString getAnswer() const;
	QDate getLastReview() const;
	QDate getNextReview() const;

	bool toReview() const;

private:
	QString question;
	QString answer;
	QDate creationDate;
	QDate lastReview;
	QDate nextReview;

	//date_lastReview ieri
	//date_nextReview domani
	//nextReviewGap (domani - ieri)^2 = 2^2 = 4
};
