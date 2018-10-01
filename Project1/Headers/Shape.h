#pragma once
#include "Entity.h"

class ENGINEDLL_API Shape : public Entity {
public:
	virtual void Draw() = 0;
	virtual void SetVertices(void* data) = 0;
};