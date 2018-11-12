#pragma once
#include "Export.h"
#include <glm.hpp>
class Entity;

struct ENGINEDLL_API BoxCollider {
	float halfWidth;
	float halfHeight;
	unsigned int mass;
	bool isStatic;
	bool isTrigger;
	Entity* pEntity;
	glm::vec3 position;
};
