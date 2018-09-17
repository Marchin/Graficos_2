#include "Game.h"
#include <iostream>

Game::Game(): m_counter (0) {
}

Game::~Game() {
	delete m_triangle;
}

bool Game::OnStart() {
	float vertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.f, 0.f
	};
	Shader program("Resources/Shaders/Shader1/vShader.glsl",
		"Resources/Shaders/Shader1/fShader.glsl");
	Material material(program);
	m_triangle = new Triangle(m_pRenderer ,material, &vertices, sizeof(vertices));
	std::cout << "Game::OnStart()" << std::endl;
	return true;
}

bool Game::OnStop() {
	std::cout << "Game::OnStop()" << std::endl;
	return true;
}

bool Game::OnUpdate() {
	m_counter++;
	m_triangle->Draw();
	//std::cout << "Loop! " << m_counter << std::endl;
	/*if (m_counter < 5) {
		return true;
	} else {
		return false;
	}*/
	return true;
}
