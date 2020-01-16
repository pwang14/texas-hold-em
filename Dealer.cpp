#include "Card.h"
#include "DeckOfCards.h"
#include "Game.h"
#include <iostream>

int main() {
	//create a Game object, play Game
	Game game;
	game.playGame();
	std::cout << std::endl << "Thanks for playing!";
}