#include "Card.h"

#ifndef DECKOFCARDS_H
#define DECKOFCARDS_H

class DeckOfCards {
private:
	Card* deck;
	//indicates the position in the deck of the first card that has not yet been dealt
	int currentCard;

public:
	const int SIZE_OF_DECK = 52;

	DeckOfCards();
	~DeckOfCards();

	//standard methods
	void shuffle();
	Card dealCard();
	bool moreCards();
};

#endif