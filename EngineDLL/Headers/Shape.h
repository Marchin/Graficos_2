#pragma once
#include "Entity.h"

class ENGINEDLL_API Shape : public Entity {
public:
	virtual void Draw() = 0;
};