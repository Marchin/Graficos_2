#pragma once
#include "Headers/CollisionManager.h"
#include "Headers/SpriteSheet.h"

class Character : public SpriteSheet{
public:
	Character(Renderer* pRenderer);
	~Character();

private:
	SpriteSheet* m_pSpriteSheet;
	BoxCollider* m_pBoxCollider;
};