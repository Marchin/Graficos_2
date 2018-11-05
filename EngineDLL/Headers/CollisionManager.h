#pragma once
#include "Export.h"

class ENGINEDLL_API  CollisionManager {
public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager GetInstance();
private:
	static CollisionManager* m_instance;
};
