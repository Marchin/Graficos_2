#pragma once
#include <array>
#include "Export.h"
#include "BoxCollider.h"

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
	std::array<BoxCollider, 64> m_colliders;
	unsigned int m_count;
};
