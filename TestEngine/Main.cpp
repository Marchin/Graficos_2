#include "Game.h"
#include <iostream>

int main() {
	GameBase* game = new Game();

	if (game->Start()) {
		game->Loop();
	}

	game->Stop();

	delete game;

	std::cin.get();
}