#pragma once
#include "Export.h"
#include <glm/glm.hpp>
class Entity;

struct ENGINEDLL_API BoxCollider {
	float halfWidth;
	float halfHeight;
	float mass = 1.f;
	bool isStatic = false;
	bool isTrigger = false;
	bool isActive = false;
	Entity* pEntity;
	glm::vec3 position;
};
