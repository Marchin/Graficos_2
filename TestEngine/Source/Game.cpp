#include "../Headers/Game.h"

Game::Game() : m_counter(0), m_sidesCounter(3), m_camX(0.f), m_camY(0.f) {
}

Game::~Game() {
	delete m_pTriangle;
	delete m_pColorSquare;
	delete m_pCircle;
	delete m_pSprite;
	delete m_pSpriteSheet;
	delete m_pCharacter;
	delete m_pCharacter2;
	delete m_pTilemap;
	delete m_pTileset;
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

	Shader sSprite("Resources/Shaders/ShaderTilemap/vTilemap.glsl",
		"Resources/Shaders/ShaderTilemap/fTilemap.glsl");
	Material materialSprite(sSprite);

	/*m_pTriangle = new Triangle(m_pRenderer, materialShape, &vertices, sizeof(vertices));
	m_pColorSquare = new ColorSquare(m_pRenderer, materialSquare, &squareVertices, &squareColors);
	m_pCircle = new Circle(m_pRenderer, materialShape, m_sidesCounter);
	const char* spritePath = "Resources/Small-mario.png";
	m_pSprite = new Sprite(m_pRenderer, materialSprite, spritePath, &squareVertices, &squareUV);*/
	const char* spriteSheetPath = "Resources/spriteSheet2.png";
	m_pSpriteSheet = new SpriteSheet(m_pRenderer, materialSprite, spriteSheetPath, &squareVertices, &squareUV);
	m_pSpriteSheet->SetFrameSize(64);

	m_pCharacter = new Character(m_pRenderer);
	m_pCharacter->Move(20.f, -9.f);
	m_pCharacter2 = new Character(m_pRenderer);
	m_pCharacter2->SetMass(5.f);
	m_pCharacter2->Move(9.f, 0.f);
	//m_pCharacter2->SetStatic(true);

	const char* tilesetPath = "Resources/tileset.png";
	m_pTileset = new SpriteSheet(m_pRenderer, materialSprite, tilesetPath, &squareVertices, &squareUV);
	m_pTileset->SetFrameSize(32);
	m_pTilemap = new Tilemap("Resources/tileset.csv", m_pTileset, m_pRenderer, sSprite);
	int collisionableTiles[] = { 37, 45, 46, 47, 55 };
	m_pTilemap->SetCollisionableTiles(collisionableTiles, sizeof(collisionableTiles)/sizeof(int));

	m_pTilemap->RegisterColliders(m_pCharacter2->GetCollider());
	m_pTilemap->RegisterColliders(m_pCharacter->GetCollider());

	printf("Game::OnStart()\n");
	return true;
}

bool Game::OnStop() {
	printf("Game::OnStop()\n");
	return true;
}

bool Game::OnUpdate(float deltaTime) {
	m_counter++;
	/*
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
	m_pSpriteSheet->Draw();
	*/
	if (m_camX > 32.f) {
		m_camX = 0.f;
		m_camY -= 5.f;
		if (m_camY < -35.f) {
			m_camY = 0.f;
		}
	} else {
		m_camX += 2.f  * deltaTime;
	}
	m_pRenderer->SetCameraPosition(m_camX, m_camY);
	m_pTilemap->Draw();
	m_pCharacter->Move(-2.f * deltaTime, 0.f);
	m_pCharacter->Update(deltaTime);
	m_pCharacter->Draw();
	m_pCharacter2->Move(0.f, -2.f * deltaTime);
	m_pCharacter2->Draw();
	m_pTilemap->CheckCollisions();
	return true;
}
