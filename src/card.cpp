#include "card.h"

Card::Card(std::string question, std::string answer)
	: question(std::move(question)), answer(std::move(answer))
{}

std::string Card::getQuestion(){
	return question;
}

std::string Card::getAnswer(){
	return answer;
}
