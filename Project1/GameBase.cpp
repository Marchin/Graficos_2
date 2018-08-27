#include "GameBase.h"
#include <iostream>

GameBase::GameBase() 
	: m_renderer(new Renderer), m_window(new Window(0,0,"")){
}

GameBase::~GameBase() {
	if (m_renderer) {
		delete m_renderer;
	}
	if (m_window) {
		delete m_window;
	}
}

bool GameBase::Start() {
	std::cout << "GameBase::Start()" << std::endl;
	m_window = new Window(0, 0, "");
	m_renderer = new Renderer; 
	if (!m_window->Start()) {
		return false;
	} else if (!m_renderer->Start()) {
		return false;
	} else {
		return OnStart();
	}
}

bool GameBase::Stop() {
	std::cout << "GameBase::Stop()" << std::endl;
	OnStop();
	m_renderer->Stop();
	m_window->Stop();
	return true;
}

void GameBase::Loop() {
	bool exit = false;
	while (!exit) {
		exit = !OnUpdate();
	}
}
