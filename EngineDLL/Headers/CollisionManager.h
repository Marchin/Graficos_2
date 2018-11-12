#pragma once
#include <vector>
#include "Export.h"
#include "BoxCollider.h"

class ENGINEDLL_API  CollisionManager {
public:
	~CollisionManager();
	static CollisionManager GetInstance();
	void Update();
	BoxCollider* Register();
	void Remove(BoxCollider* collider);
private:
	void CheckCollision(int index1, int index2);
	std::vector<BoxCollider> m_colliders;
	CollisionManager();
};
