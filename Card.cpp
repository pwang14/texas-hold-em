#include "Card.h"
#include <string>

std::string Card::faces[13] = { "Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King","Ace" };

std::string Card::suits[4] = { "Spades","Clubs","Diamonds","Hearts" };

Card::Card() {
	this->face = 0;
	this->suit = 0;
}

Card::Card(int _face, int _suit) {
	this->face = _face;
	this->suit = _suit;
}

void Card::operator=(const Card& c) {
	this->face = c.getFace();
	this->suit = c.getSuit();
}

bool operator==(const Card& c, const Card& k) {
	if (c.getFace() == k.getFace() && c.getSuit() == k.getSuit()) return true;
	else return false;
}

int Card::getFace() const {
	return face;
}

int Card::getSuit() const {
	return suit;
}

void Card::setFace(int _face) {
	this->face = _face;
}

void Card::setSuit(int _suit) {
	this->suit = _suit;
}

std::string Card::toString() {
	return Card::faces[face] + " of " + Card::suits[suit];
}

std::string Card::printHand(Card* hand, int length) {
	std::string output;
	for (int i = 0; i < length - 1; i++) {
		output += hand[i].toString() + ", ";
	}
	output += hand[length - 1].toString();
	return output;
}

void Card::transferHand(Card* hand1, Card* hand2, int length) {
	for (int i = 0; i < length; i++) {
		hand2[i] = hand1[i];
	}
}

void Card::orgHand(Card* hand, int length) {
	int count = 0;
	Card* tempHand = new Card[length];
	for (int i = 0; i < 13; i++) {
		for (int k = 0; k < length; k++) {
			if (hand[k].getFace() == i) {
				tempHand[count] = hand[k];
				count++;
			}
		}
	}
	Card::transferHand(tempHand, hand, length);
	delete[] tempHand;
}

//outputs the type of hand
int Card::checkHand(Card* _hand, int length) {
	Card* hand = new Card[length];
	transferHand(_hand, hand, length);
	Card::orgHand(hand, length);
	int type = -1;

	bool flush = true;
	bool straight = true;

	if (length < 5) {
		flush = false;
		straight = false;
	}
	for (int i = 0; i < length - 1; i++) {
		if (hand[i].getSuit() != hand[i + 1].getSuit()) flush = false;
		if (hand[i].getFace() != hand[i + 1].getFace() - 1) straight = false;
	}
	if (length == 5 && hand[0].getFace() == 0 && hand[1].getFace() == 1 && hand[2].getFace() == 2 && hand[3].getFace() == 3 && hand[4].getFace() == 12) {
		straight = true;
		Card tempCard = hand[4];
		hand[4] = hand[3];
		hand[3] = hand[2];
		hand[2] = hand[1];
		hand[1] = hand[0];
		hand[0] = tempCard;
	}

	int groups[3] = { 0,0,0 };

	int highestGroup = -1;
	int count = 0;
	for (int i = 1; i < length; i++) {
		if (hand[i].getFace() == hand[i - 1].getFace()) {
			count++;
			if (i == length - 1) {
				groups[count - 1] ++;
				if (count == 3 || groups[count] == 0) highestGroup = hand[i - 1].getFace();
			}
		}
		else {
			if (count > 0) {
				groups[count - 1] ++;
				if (count == 3 || groups[count] == 0) highestGroup = hand[i - 1].getFace();
				count = 0;
			}
		}
	}

	if (straight && flush) {
		if (hand[0].getFace() == 8) type = 912;
		else type = 800 + hand[length - 1].getFace();
	}
	else if (groups[2] > 0) type = 700 + hand[1].getFace();
	else if (flush) type = 500 + hand[length - 1].getFace();
	else if (straight) type = 400 + hand[length - 1].getFace();
	else if (groups[1] > 0) {
		if (groups[0] == 1) type = 600 + hand[2].getFace();
		else type = 300 + hand[2].getFace();
	}
	else if (groups[0] == 2) type = 200 + highestGroup;
	else if (groups[0] == 1) type = 100 + highestGroup;
	else type = hand[length - 1].getFace();

	delete[] hand;
	return type;
}

//determines the winner out of two hands, uses recursion for ties (remove tied cards, compare again)
int Card::compareHands(Card* hand1, Card* hand2, int length) {
	int type1 = Card::checkHand(hand1, length);
	int type2 = Card::checkHand(hand2, length);
	if (type1 > type2) {
		return 1;
	}
	else if (type2 > type1) {
		return -1;
	}
	else {
		Card* copyHand1 = new Card[length];
		Card* copyHand2 = new Card[length];
		int count1 = 0;
		int count2 = 0;
		for (int i = 0; i < length; i++) {
			if (hand1[i].face != type1 % 100) {
				copyHand1[count1] = hand1[i];
				count1++;
			}
			if (hand2[i].face != type2 % 100) {
				copyHand2[count2] = hand2[i];
				count2++;
			}
		}
		if (count1 == 0) {
			delete[] copyHand1;
			delete[] copyHand2;
			return 0;
		}
		else {
			int compareVal = Card::compareHands(copyHand1, copyHand2, count1);
			delete[] copyHand1;
			delete[] copyHand2;
			return compareVal;
		}
	}
}