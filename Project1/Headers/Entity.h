#pragma once
#include "Export.h"
#include <glm.hpp>

class ENGINEDLL_API Entity {
public:
	Entity();
	~Entity() {}
	void SetPosition(float x, float y, float z);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(float x, float y, float z);
	glm::mat4 GetModelMatrix() const;
	glm::mat4 GetModel() const;
private:
	glm::mat4 m_model;
	glm::mat4 m_position;
	glm::mat4 m_rotation;
	glm::mat4 m_scale;

	glm::vec3 m_translate;
	glm::vec3 m_rotationX;
	glm::vec3 m_rotationY;
	glm::vec3 m_rotationZ;
};
