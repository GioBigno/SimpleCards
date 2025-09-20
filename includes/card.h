#ifndef CARD_H
#define CARD_H

#include <string>

class Card{
public:
	Card(std::string question, std::string answer);

	std::string getQuestion();
	std::string getAnswer();

private:
	std::string question;
	std::string answer;
};

#endif //CARD_H
