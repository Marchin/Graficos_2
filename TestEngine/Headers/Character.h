#pragma once
#include "Headers/CollisionManager.h"
#include "Headers/SpriteSheet.h"

class Character
{
public:
	Character();
	~Character();

private:
	SpriteSheet* m_spriteSheet;
	BoxCollider* m_boxCollider;
};