#include "Card.h"
#include "DeckOfCards.h"
#include <string>

#ifndef GAME_H
#define GAME_H

class Game {
private:
	//mp, mc are the amounts of money for player and computer
	int mp;
	int mc;
	int startP;
	//specific 2-card hands
	Card* playerHand;
	Card* compHand;
	//common hand
	Card* dealtHand;
	DeckOfCards deck;
	//regulates how bold the computer is
	int safety;

	void playRound(int);

	//display the winner
	void displayWin(int, int*);
	//determine who wins at the end
	int determineWin();
	//ends the game prematurely incase of an all in
	void endGame(int, int*);
	//finds the best hand out of a 2-card and 5-card hand (e.g. playerHand and dealtHand)
	Card* bestHand(Card*, Card*);
	//finds all possible combinations of k elements, out of a total n elements (n choose k)
	void possibleCombinations(int**, int*, int[], int, int, int, int);

	//runs a round of betting
	int roundBet(int*, int, bool, int, int, int);
	//manages the money when a player goes all in
	int allIn(int, int*);
	//receives the bets, ensures proper format
	int inputBet(int, int, int, int, bool, int);
	//receives bets from the player
	int inputBetP();
	//receives bets from the computer
	//the computer considers all possible combinations of the dealt cards and the remaining, unknown cards
	//uses probability to decide how much to bet
	int inputBetC(int, int, int, bool, int);

public:
	Game();
	Game(int, int, int, int);
	~Game();

	void playGame();
};

#endif