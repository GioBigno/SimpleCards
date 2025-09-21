#include "card.h"

Card::Card(QString question, QString answer)
	: question(std::move(question)), answer(std::move(answer))
{}

QString Card::getQuestion(){
	return question;
}

QString Card::getAnswer(){
	return answer;
}
