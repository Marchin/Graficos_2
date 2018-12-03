#pragma once
#include "Headers/CollisionManager.h"
#include "Headers/Animation.h"

class Character {
public:
	Character(Renderer* pRenderer);
	~Character();
	void Move(float x, float y);
	void Draw();
	void SetStatic(bool state);
	void Update(float deltaTime);
	void SetMass(float mass);
	BoxCollider* GetCollider();
private:
	float m_speed;
	SpriteSheet* m_pSpriteSheet;
	BoxCollider* m_pBoxCollider;
	Animation* m_pAnimation;
};