#include "../Headers/GameBase.h"
#include "../Headers/CollisionManager.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

GameBase::GameBase() : lastFrame(0.f), deltaTime(0.f) {
}

GameBase::~GameBase() {
}

bool GameBase::Start() {
	printf("GameBase::Start()\n");
	m_pWindow = new Window;
	m_pRenderer = new Renderer; 
	if (!m_pWindow->Start(800, 600, "Engine")) {
		return false;
	} else if (!m_pRenderer->Start()) {
		return false;
	} else {
		return OnStart();
	}
}

bool GameBase::Stop() {
	printf("GameBase::Stop()\n");
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
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		//printf("%f\n", 1.f / deltaTime);
		lastFrame = currentFrame;
		m_pWindow->PollEvents();
		m_pRenderer->Clear();
		m_pRenderer->FillColor(0.1f, 0.1f, 0.1f);
		CollisionManager::GetInstance()->Update();
		exit = !OnUpdate(deltaTime);
		m_pRenderer->SwapBuffers(m_pWindow->GetWindow());
	}
}
