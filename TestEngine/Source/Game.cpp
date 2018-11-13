#include "../Headers/Game.h"
#include <iostream>

Game::Game(): m_counter (0), m_sidesCounter(3) {
}

Game::~Game() {
	delete m_pTriangle;
	delete m_pColorSquare;
	delete m_pCircle;
	delete m_pSprite;
	delete m_pSpriteSheet;
	delete m_pCharacter;
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
	float squareUV[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f
	};
	Shader sShape("Resources/Shaders/Shader1/vShader.glsl",
		"Resources/Shaders/Shader1/fShader.glsl");
	Material materialShape(sShape);
	
	Shader sColorSquare("Resources/Shaders/ColorShader/vColor.glsl",
		"Resources/Shaders/ColorShader/fColor.glsl");
	Material materialSquare(sColorSquare);

	Shader sSprite("Resources/Shaders/ShaderTexture/vTexture.glsl",
		"Resources/Shaders/ShaderTexture/fTexture.glsl");
	Material materialSprite(sSprite);

	m_pTriangle = new Triangle(m_pRenderer, materialShape, &vertices, sizeof(vertices));
	m_pColorSquare = new ColorSquare(m_pRenderer, materialSquare, &squareVertices, &squareColors);
	m_pCircle = new Circle(m_pRenderer, materialShape, m_sidesCounter);
	const char* spritePath = "Resources/Small-mario.png";
	const char* spriteSheetPath = "Resources/spriteSheet2.png";
	m_pSprite = new Sprite(m_pRenderer, materialSprite, spritePath, &squareVertices, &squareUV);
	m_pSpriteSheet = new SpriteSheet(m_pRenderer, materialSprite, spriteSheetPath, &squareVertices, &squareUV);
	m_pSpriteSheet->SetFrameSize(64);
	m_pCharacter = new Character(m_pRenderer, &squareVertices);
	m_pCharacter2 = new Character(m_pRenderer, &squareVertices);

	std::cout << "Game::OnStart()" << std::endl;
	return true;
}

bool Game::OnStop() {
	std::cout << "Game::OnStop()" << std::endl;
	return true;
}

bool Game::OnUpdate() {

	/*m_counter++;
	m_pTriangle->SetPosition(-2.0f, -2.0f, 0.0f);
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
		m_pColorSquare->SetVertices(&squareVertices);
		m_pTriangle->SetVertices(&squareVertices);
	}

	m_pColorSquare->RotateY(sin(m_counter*0.001f) * 180.f / 3.1415f);
	m_pColorSquare->Scale(3.f, 3.f, 3.f);
	m_pColorSquare->SetPosition(2.f, 2.f, 0.f);
	m_pColorSquare->Draw();

	if (m_counter % 25 == 0) {
		m_sidesCounter++;
		m_sidesCounter %= 15;
	}
	m_pCircle->SetSidesAmount(m_sidesCounter);
	m_pCircle->SetRadius(m_sidesCounter / 10.f);
	m_pCircle->SetPosition(-2.f, 2.f, 0.f);
	m_pCircle->Draw();

	m_pSprite->SetPosition(2.f, -2.f, 0.f);
	m_pSprite->Scale(3.f, 3.f, 1.f);
	m_pSprite->Draw();

	if (m_counter == 1000) {
		m_counter = 0;
	}

	if (m_counter % 125 == 0) {
		m_pSpriteSheet->SetFrame(m_counter / 125);
	}

	m_pSpriteSheet->Scale(3.f, 3.f, 1.f);
	m_pSpriteSheet->Draw();*/
	return true;
}
