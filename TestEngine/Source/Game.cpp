#include "../Headers/Game.h"
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
		 0.5f, -0.5f, 0.f,
		 0.5f,  0.5f, 0.f
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
	
	Shader sColorSquare("Resources/Shaders/ColorShader/vColor.glsl",
		"Resources/Shaders/ColorShader/fColor.glsl");
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
	m_pTriangle->Translate(0.0f, 0.0f, 0.0f);
	m_pTriangle->Scale(sin(m_counter*0.01f), 1.f, sin(m_counter*0.01f));
	m_pTriangle->Scale(3.f, sin(m_counter*0.01f) * 3.f, 1.f);
	m_pTriangle->RotateX(sin(m_counter*0.01f) * 180.f / 3.1415f);
	m_pTriangle->RotateY(sin(m_counter*0.01f) * 180.f / 3.1415f);
	m_pTriangle->RotateZ(sin(m_counter*0.01f) * 180.f / 3.1415f);
	m_pTriangle->Draw();
	if (m_counter == 255) {
		float squareColors[] = {
			1.f, 0.f, 0.f,
			1.f, 0.f, 1.f,
			1.f, 1.f, 1.f,
			0.f, 1.f, 0.f
		};
		m_pColorSquare->SetColors(&squareColors);
	}


	if (m_counter == 125) {
		float squareVertices[] = {
			-1.0f, -0.5f, 0.f,
			-0.5f,  0.5f, 0.f,
			 0.5f, -0.5f, 0.f,
			 0.5f,  1.0f, 0.f
		};
		m_pColorSquare->SetPoints(&squareVertices);
	}

	m_pColorSquare->RotateY(sin(m_counter*0.001f) * 180.f / 3.1415f);
	m_pColorSquare->Scale(3.f, 3.f, 3.f);
	m_pColorSquare->SetPosition(2.f, 2.f, 0.f);
	m_pColorSquare->Draw();
	return true;
}
