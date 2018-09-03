#include "GameBase.h"
#include <iostream>

GameBase::GameBase() {
}

GameBase::~GameBase() {
}

bool GameBase::Start() {
	std::cout << "GameBase::Start()" << std::endl;
	m_pWindow = new Window;
	m_pRenderer = new Renderer; 
	if (!m_pWindow->Start(800, 600, "")) {
		return false;
	} else if (!m_pRenderer->Start()) {
		return false;
	} else {
		return OnStart();
	}
}

bool GameBase::Stop() {
	std::cout << "GameBase::Stop()" << std::endl;
	OnStop();
	m_pRenderer->Stop();
	m_pWindow->Stop();
	delete m_pRenderer;
	delete m_pWindow;
	return true;
}

void GameBase::Loop() {
	bool exit = false;
	while (!exit && !m_pWindow->ShouldClose()) {
		exit = !OnUpdate();
		m_pWindow->PollEvents();
		m_pRenderer->Clear();
		m_pRenderer->FillColor(0.f, 0.f, 1.f);
		m_pRenderer->SwapBuffers(m_pWindow->GetWindow());
	}
}
