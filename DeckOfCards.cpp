#include "DeckOfCards.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

DeckOfCards::DeckOfCards() {
	deck = new Card[SIZE_OF_DECK];
	int count = 0;
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 13; k++) {
			deck[count] = Card(k, i);
			count++;
		}
	}
	currentCard = 0;
}

DeckOfCards::~DeckOfCards() {
	delete[] deck;
}

void DeckOfCards::shuffle() {
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 52; i++) {
		int n = rand() % 52;
		Card tempCard = deck[i];
		deck[i] = deck[n];
		deck[n] = tempCard;
	}
	currentCard = 0;
}

Card DeckOfCards::dealCard() {
	if (!moreCards()) {
		std::cout << "No more cards, reshuffling deck.";
		this->shuffle();
	}
	currentCard++;
	return deck[currentCard - 1];
}

bool DeckOfCards::moreCards() {
	if (currentCard < 52) return true;
	else return false;
}
