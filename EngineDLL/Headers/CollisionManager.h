#pragma once
#include "Export.h"
#include "BoxCollider.h"
#define COLLIDERS_AMOUNT 64

class ENGINEDLL_API  CollisionManager {
public:
	~CollisionManager();
	static CollisionManager* GetInstance();
	void Update();
	BoxCollider* Register();
	void Remove(BoxCollider* collider);
private:
	CollisionManager();
	inline void CheckCollision(int index1, int index2);
	BoxCollider m_colliders[COLLIDERS_AMOUNT];
	uint8_t m_count;
};
