#include <string>

#ifndef CARD_H
#define CARD_H

class Card {
private:
	int face;
	int suit;

public:
	static std::string faces[];
	static std::string suits[];

	Card();
	Card(int, int);

	//overloaded operators for assigning and comparing
	void operator=(const Card& c);
	friend bool operator==(const Card& c, const Card& k);

	//standard methods
	int getFace() const;
	int getSuit() const;
	void setFace(int);
	void setSuit(int);
	std::string toString();

	//methods for arrays of cards (hands)
	static std::string printHand(Card* hand, int length);
	static void transferHand(Card* hand1, Card* hand2, int length);
	static void orgHand(Card* hand, int length);

	//outputs a score for a hand
	static int checkHand(Card* hand, int length);

	//sees which hand is better
	static int compareHands(Card* hand1, Card* hand2, int length);
};

#endif