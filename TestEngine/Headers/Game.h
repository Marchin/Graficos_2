#pragma once
#include "Headers/GameBase.h"
#include "Headers/Triangle.h"
#include "Headers/ColorSquare.h"
#include "Headers/Circle.h"
#include "Headers/Sprite.h"
#include "Headers/SpriteSheet.h"
#include "Headers/Tilemap.h"
#include "Character.h"

class Game: public GameBase {
public:
	Game();
	~Game();
	
private:
	bool OnStart() override;
	bool OnStop() override;
	bool OnUpdate(float deltaTime) override;
	int m_counter;
	int m_sidesCounter;
	float m_camX;
	float m_camY;
	Triangle* m_pTriangle;
	ColorSquare* m_pColorSquare;
	Circle* m_pCircle;
	Sprite* m_pSprite;
	SpriteSheet* m_pSpriteSheet;
	Character* m_pCharacter;
	Character* m_pCharacter2;
	Character* m_pCharacter3;
	SpriteSheet* m_pTileset;
	Tilemap* m_pTilemap;
};
