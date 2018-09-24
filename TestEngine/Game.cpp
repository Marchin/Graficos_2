#include "Game.h"
#include <iostream>

Game::Game(): m_counter (0) {
}

Game::~Game() {
	delete m_pTriangle;
}

bool Game::OnStart() {
	float vertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.f, 0.f
	};
	float squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  -0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};
	float squareColors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 1.f, 0.f
	};
	Shader sTriangle("Resources/Shaders/Shader1/vShader.glsl",
		"Resources/Shaders/Shader1/fShader.glsl");
	Material materialTriangle(sTriangle);
	Shader sColorSquare("Resources/Shaders/Shader1/vShader.glsl",
		"Resources/Shaders/Shader1/fShader.glsl");
	Material materialSquare(sColorSquare);
	m_pTriangle = new Triangle(m_pRenderer, materialTriangle, &vertices, sizeof(vertices));
	m_pColorSquare = new ColorSquare(m_pRenderer, materialSquare, &squareVertices, &squareColors);
	std::cout << "Game::OnStart()" << std::endl;
	return true;
}

bool Game::OnStop() {
	std::cout << "Game::OnStop()" << std::endl;
	return true;
}

bool Game::OnUpdate() {
	m_counter++;
	//m_triangle->Translate(sin(m_counter*0.01f), 1.f, sin(m_counter*0.01f));
	//m_triangle->Scale(sin(m_counter*0.01f), 1.f, sin(m_counter*0.01f));
	m_pTriangle->Scale(3.f, sin(m_counter*0.01f) * 3.f, 1.f);
	//m_triangle->Translate(-sin(m_counter*0.01f), 0.f, 0.f);
	m_pTriangle->RotateX(sin(m_counter*0.01f) * 180.f / 3.1415f);
	m_pTriangle->RotateY(sin(m_counter*0.01f) * 180.f / 3.1415f);
	m_pTriangle->RotateZ(sin(m_counter*0.01f) * 180.f / 3.1415f);
	//m_pTriangle->Draw();
	m_pColorSquare->Scale(3.f, 3.f, 3.f);
	m_pColorSquare->Draw();
	//std::cout << "Loop! " << m_counter << std::endl;
	/*if (m_counter < 5) {
		return true;
	} else {
		return false;
	}*/
	return true;
}
