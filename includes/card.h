#ifndef CARD_H
#define CARD_H

#include <QString>

class Card{
public:
	Card(QString question, QString answer);

	QString getQuestion();
	QString getAnswer();

private:
	QString question;
	QString answer;
};

#endif //CARD_H
