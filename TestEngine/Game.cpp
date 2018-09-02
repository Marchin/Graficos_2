#include "Game.h"
#include <iostream>

Game::Game(): m_counter (0) {
}

Game::~Game() {
}

bool Game::OnStart() {
	std::cout << "Game::OnStart()" << std::endl;
	return true;
}

bool Game::OnStop() {
	std::cout << "Game::OnStop()" << std::endl;
	return true;
}

bool Game::OnUpdate() {
	m_counter++;
	//std::cout << "Loop! " << m_counter << std::endl;
	/*if (m_counter < 5) {
		return true;
	} else {
		return false;
	}*/
	return true;
}
