#include "Game.h"
#include "Card.h"
#include "DeckOfCards.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <math.h>

Game::Game() {
	mp = 100;
	mc = 100;
	startP = 1;
	safety = 5;
	playerHand = new Card[2];
	compHand = new Card[2];
	dealtHand = new Card[5];
}

Game::Game(int _mp, int _mc, int _startP, int _safety) {
	mp = _mp;
	mc = _mc;
	startP = _startP;
	safety = _safety;
	playerHand = new Card[2];
	compHand = new Card[2];
	dealtHand = new Card[5];
}

Game::~Game() {
	delete[] playerHand;
	delete[] compHand;
	delete[] dealtHand;
}

//play a game of poker, end when someone loses all their money
void Game::playGame() {
	std::cout << "Texas Hold'Em" << std::endl << std::endl;
	std::cout << "You will play against the computer. Both of you start off with $100, and you can make $5 increment bets." << std::endl;
	std::cout << "On your turn, you can type 'f' to fold, 'c' to call, or 'r' to raise (you can specify how much you want to raise after)." << std::endl;
	std::cout << "Small blind switches every round, must make a $5 bet at the beginning." << std::endl;
	std::cout << "The computer is not a very good player, and might make some questionable moves. Just go along with it." << std::endl;

	deck.shuffle();
	do {
		std::cout << std::endl << "---------------------" << std::endl;
		std::cout << "Starting a new round." << std::endl << "You: $" << mp << ", Computer: $" << mc << "." << std::endl << std::endl;
		playRound(startP);
		startP = (startP + 1) % 2;
	} while (mp > 0 && mc > 0);
	if (mp == 0) std::cout << "The computer won!" << std::endl;
	else std::cout << "You won against the computer!" << std::endl;
}

//play a single round, 4 rounds of betting
void Game::playRound(int _turn) {
	if (_turn == 0) std::cout << "You are the small blind." << std::endl << std::endl;
	else std::cout << "Computer is small blind" << std::endl << std::endl;

	int* money = new int[4];
	money[0] = mp;
	money[1] = mc;
	money[2] = 0;
	money[3] = 0;

	srand((unsigned int)time(NULL));
	int extraRoll = rand() % 100;

	playerHand[0] = deck.dealCard();
	playerHand[1] = deck.dealCard();
	std::cout << "Your hand: " << Card::printHand(playerHand, 2) << std::endl << std::endl;
	compHand[0] = deck.dealCard();
	compHand[1] = deck.dealCard();
	int status = roundBet(money, _turn, true, 0, this->safety, extraRoll);
	if (status >= 0) {
		displayWin(status, money);
		return;
	}
	else if (status == -2) {
		endGame(0, money);
		return;
	}
	std::cout << std::endl;

	dealtHand[0] = deck.dealCard();
	dealtHand[1] = deck.dealCard();
	dealtHand[2] = deck.dealCard();
	std::cout << "Your hand: " << Card::printHand(playerHand, 2) << std::endl;
	std::cout << "Dealt cards: " << Card::printHand(dealtHand, 3) << std::endl << std::endl;
	status = roundBet(money, _turn, false, 3, this->safety, extraRoll);
	if (status >= 0) {
		displayWin(status, money);
		return;
	}
	else if (status == -2) {
		endGame(3, money);
		return;
	}
	std::cout << std::endl;

	dealtHand[3] = deck.dealCard();
	std::cout << "Your hand: " << Card::printHand(playerHand, 2) << std::endl;
	std::cout << "Dealt cards: " << Card::printHand(dealtHand, 4) << std::endl << std::endl;

	status = roundBet(money, _turn, false, 4, this->safety, extraRoll);
	if (status >= 0) {
		displayWin(status, money);
		return;
	}
	else if (status == -2) {
		endGame(4, money);
		return;
	}
	std::cout << std::endl;

	dealtHand[4] = deck.dealCard();
	std::cout << "Your hand: " << Card::printHand(playerHand, 2) << std::endl;
	std::cout << "Dealt cards: " << Card::printHand(dealtHand, 5) << std::endl << std::endl;

	status = roundBet(money, _turn, false, 5, this->safety, extraRoll);
	if (status >= 0) {
		displayWin(status, money);
		return;
	}

	std::cout << std::endl << "Dealt cards: " << Card::printHand(dealtHand, 5) << std::endl << std::endl;
	status = determineWin();
	displayWin(status, money);
}

//display the winner, give the pot to the winner
void Game::displayWin(int status, int* money) {
	std::cout << std::endl;
	if (status == 0) {
		std::cout << "You won a pot of $" << money[2] + money[3] << "." << std::endl;
		mp += money[3];
		mc = money[1];
		std::cout << "You: $" << mp << ", Computer: $" << mc << "." << std::endl;
	}
	else if (status == 1) {
		std::cout << "Computer won a pot of $" << money[2] + money[3] << "." << std::endl;
		mp = money[0];
		mc += money[2];
		std::cout << "You: $" << mp << ", Computer: $" << mc << "." << std::endl;
	}
	else if (status == 3) {
		std::cout << "Both of you tied. The money will be split.";
		std::cout << "You: $" << mp << ", Computer: $" << mc << "." << std::endl;
	}
}

//determine which hand wins, considers the 5 dealt cards in the middle as well
int Game::determineWin() {
	Card* hand1 = bestHand(playerHand, dealtHand);
	Card* hand2 = bestHand(compHand, dealtHand);
	std::cout << "Your hand: " << Card::printHand(playerHand, 2) << std::endl << "Computer hand: " << Card::printHand(compHand, 2) << std::endl;
	int winner = Card::compareHands(hand1, hand2, 5);
	if (winner == 1) return 0;
	else if (winner == -1) return 1;
	else return 3;
}

//skips the rounds of betting if someone goes all in
void Game::endGame(int numDealt, int* money) {
	for (int i = numDealt; i < 5; i++) {
		dealtHand[i] = deck.dealCard();
	}
	std::cout << std::endl << "Dealt cards: " << Card::printHand(dealtHand, 5) << std::endl << std::endl;
	int endStatus = determineWin();
	displayWin(endStatus, money);
}

//determines the best possible hand out of a 2-card hand (hand) and a 5-card hand (extraHand)
Card* Game::bestHand(Card* hand, Card* extraHand) {
	int** combinations = new int* [21];
	for (int i = 0; i < 21; i++) {
		combinations[i] = new int[5];
	}
	int data[5];
	int count = 0;
	possibleCombinations(combinations, &count, data, 0, 6, 0, 5);

	Card* bestHand = new Card[5];
	bestHand[0] = hand[0];
	bestHand[1] = hand[1];
	bestHand[2] = extraHand[0];
	bestHand[3] = extraHand[1];
	bestHand[4] = extraHand[2];

	Card* tempHand = new Card[5];

	for (int i = 1; i < 21; i++) {
		for (int k = 0; k < 5; k++) {
			if (combinations[i][k] > 1) tempHand[k] = extraHand[combinations[i][k] - 2];
			else tempHand[k] = hand[combinations[i][k]];
		}
		if (Card::compareHands(tempHand, bestHand, 5) == 1) {
			Card::transferHand(tempHand, bestHand, 5);
		}
	}
	delete[] tempHand;
	return bestHand;
}

//determines all possible combinations of k elements out of a total n elements (n choose k)
void Game::possibleCombinations(int** list, int* count, int data[], int start, int end, int index, int r) {
	if (index == r) {
		for (int i = 0; i < r; i++) {
			list[*count][i] = data[i];
		}
		*count += 1;
		return;
	}

	for (int i = start; i <= end && end - i + 1 >= r - index; i++) {
		data[index] = i;
		possibleCombinations(list, count, data, i + 1, end, index + 1, r);
	}
}

//runs a round of betting
int Game::roundBet(int* money, int _turn, bool firstRound, int numDealt, int safety, int extraRoll) {
	bool blinds = firstRound;
	bool firstBet = true;
	int turn = _turn;
	int bet = 0;
	int isAllIn = 0;
	int minCount = 0;
	do {
		if (blinds) {
			blinds = false;
			money[turn] -= 5;
			money[turn + 2] += 5;
			if (turn == 1) std::cout << "Computer bet 5 (small blind)." << std::endl;
			else std::cout << "You bet 5 (small blind)." << std::endl;
			firstBet = false;
		}
		else {
			bet = inputBet(turn, numDealt, money[(turn + 1) % 2 + 2], safety, firstBet, extraRoll);
			firstBet = false;
			if (bet == -1) {
				return (turn + 1) % 2;
			}
			else if (bet == 0) {
				if (money[(turn + 1) % 2 + 2] - money[turn + 2] >= money[turn]) {
					isAllIn = allIn(turn, money);
					if (isAllIn == 1) return -2;
				}
				else {
					money[turn] -= money[(turn + 1) % 2 + 2] - money[turn + 2];
					money[turn + 2] = money[(turn + 1) % 2 + 2];
					if (minCount >= 1) {
						if (isAllIn == 2) return -2;
						else return -1;
					}
				}
			}
			else {
				if (money[(turn + 1) % 2 + 2] - money[turn + 2] + bet >= money[turn]) {
					isAllIn = allIn(turn, money);
					if (isAllIn == 1) return -2;
				}
				else {
					money[turn] -= money[(turn + 1) % 2 + 2] - money[turn + 2] + bet;
					money[turn + 2] = money[(turn + 1) % 2 + 2] + bet;
					if (turn == 0) std::cout << "You raised by " << bet << " for a total bet of " << money[turn + 2] << std::endl;
					else std::cout << "Computer raised by " << bet << " for a total bet of " << money[turn + 2] << std::endl;
				}
			}
		}
		turn = (turn + 1) % 2;
		minCount++;
	} while (money[2] != money[3] || minCount < 2);
	return -1;
}

//manages money when someone goes all in
int Game::allIn(int _turn, int* money) {
	int turn = _turn;
	money[turn + 2] += money[turn];
	money[turn] = 0;
	if (turn == 0) std::cout << "You are all in at " << money[turn + 2] << std::endl;
	else std::cout << "Computer is all in at " << money[turn + 2] << std::endl;
	if (money[turn + 2] < money[(turn + 1) % 2 + 2]) {
		std::cout << "Making bets equal." << std::endl;
		money[(turn + 1) % 2] += money[(turn + 1) % 2 + 2] - money[turn + 2];
		money[(turn + 1) % 2 + 2] -= money[(turn + 1) % 2 + 2] - money[turn + 2];
		return 1;
	}
	else {
		return 2;
	}
}

//receives bets, ensures proper format
int Game::inputBet(int player, int numDealt, int bet, int safety, bool first, int extraRoll) {
	if (player == 0) return inputBetP();
	else return inputBetC(numDealt, bet, safety, first, extraRoll);
}

//receives bets from player
int Game::inputBetP() {
	std::cout << "It is your turn. What would you like to do? ";
	std::string input;
	bool validIn;
	do {
		validIn = true;
		std::cin >> input;
		if (input == "f") {
			std::cout << "You folded." << std::endl;
			return -1;
		}
		else if (input == "c") {
			std::cout << "You called." << std::endl;
			return 0;
		}
		else if (input == "r") {
			std::cout << "How much would you like to raise by? ";
			int bet;
			do {
				validIn = true;
				std::cin >> bet;
				if (bet % 5 == 0 && bet > 0) {
					return bet;
				}
				else {
					std::cout << "Invalid input. Bets must be multiples of 5. ";
					validIn = false;
				}
			} while (!validIn);
		}
		else {
			std::cout << "Invalid input. The valid inputs are: 'f' to fold, 'c' to check or call, 'r' to raise. " << std::endl;
			validIn = false;
		}
	} while (!validIn);
	return -1;
}

//receives bets from the computer
  //the computer considers all possible combinations of the dealt cards and the remaining, unknown cards
  //uses probability to decide how much to bet
int Game::inputBetC(int numDealt, int bet, int safety, bool first, int extraRoll) {
	int score = 0;
	int outBet = 0;
	if (numDealt == 0) {
		score = Card::checkHand(compHand, 2);
	}
	else if (numDealt == 5) {
		score = Card::checkHand(bestHand(compHand, dealtHand), 5);
	}
	else {
		Card* extraCards = new Card[50 - numDealt];
		Card tempCard;
		bool dealt;
		int count = 0;
		for (int i = 0; i < 13; i++) {
			for (int k = 0; k < 4; k++) {
				tempCard.setFace(i);
				tempCard.setSuit(k);
				dealt = false;
				if (tempCard == compHand[0] || tempCard == compHand[1]) dealt = true;
				for (int i = 0; i < numDealt; i++) {
					if (tempCard == dealtHand[i]) dealt = true;
				}
				if (!dealt) {
					extraCards[count] = tempCard;
					count++;
				}
			}
		}

		int length;
		if (numDealt == 3) length = 1081;
		else length = 46;

		int** combinations = new int* [length];
		for (int i = 0; i < length; i++) {
			combinations[i] = new int[5 - numDealt];
		}
		int data[5];
		count = 0;
		possibleCombinations(combinations, &count, data, 0, 50 - numDealt - 1, 0, 5 - numDealt);

		Card extraHand[5];
		int tempScore;
		count = 0;
		for (int i = 0; i < numDealt; i++) {
			extraHand[count] = dealtHand[i];
			count++;
		}

		long frequency[10];
		long value[10];
		int trackCompHand;
		for (int i = 0; i < length; i++) {
			for (int k = 0; k < 5 - numDealt; k++) {
				extraHand[count + k] = extraCards[combinations[i][k]];
			}
			tempScore = Card::checkHand(bestHand(compHand, extraHand), 5);
			for (int j = 0; j < 5; j++) {
				if (bestHand(compHand, extraHand)[j] == compHand[0] || bestHand(compHand, extraHand)[j] == compHand[1]) trackCompHand = tempScore / 100;
			}
			frequency[tempScore / 100]++;
			value[tempScore / 100] += tempScore % 100;
		}

		for (int i = 0; i < 10; i++) {
			int sumFreq = 0;
			for (int k = i; k < 10; k++) {
				sumFreq += frequency[k];
			}
			if (i < 3) {
				if (sumFreq > length * 0.01 * safety && i <= trackCompHand && frequency[i] > 0) score = i * 100 + value[i] / frequency[i];
			}
			else {
				if (sumFreq > length * 0.005 * safety && i <= trackCompHand && frequency[i] > 0) score = i * 100 + value[i] / frequency[i];
			}
		}
	}
	srand((unsigned int)time(NULL));
	int roll;

	int extra = 0;
	if (extraRoll < 2) extra = 5;

	if (numDealt == 0) {
		if (extraRoll > 95) score += 200;
		if (score >= 100) outBet = 10;
		else outBet = 5;
		outBet += ((int)round((score % 100) * 5.0 / 12));
		outBet = ((int)5 * round(outBet / 5.0));
		outBet += extra;
	}
	else {
		if (extraRoll > 90 && score < 800) score += 200;
		if (score / 100 == 0) outBet = 5;
		else if (score / 100 == 1) outBet = 10;
		else if (score / 100 == 2) outBet = 20;
		else if (score / 100 == 3) outBet = 30;
		else if (score / 100 == 4) outBet = 35;
		else if (score / 100 == 5) outBet = 40;
		else if (score / 100 == 6) outBet = 70;
		else if (score / 100 == 7) outBet = 80;
		else if (score / 100 == 8) outBet = 100;
		else if (score / 100 == 9) outBet = 100;
		if (numDealt == 3 && outBet >= 20) outBet = ((int)5 * round(outBet / 10.0));
		outBet += ((int)round(score % 100) * 5.0 / 12);
		outBet = ((int)5 * round(outBet / 5.0));
		outBet += extra;
	}
	if (outBet < bet - 10) {
		roll = rand() % 100;
		if (roll < 10 || (first && numDealt > 0)) {
			std::cout << "Computer called. " << std::endl;
			return 0;
		}
		else {
			std::cout << "Computer folded." << std::endl;
			return -1;
		}
	}
	else if (outBet <= bet + 10) {
		std::cout << "Computer called." << std::endl;
		return 0;
	}
	else {
		return outBet - bet;
	}
}