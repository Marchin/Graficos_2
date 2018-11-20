#pragma once
#include "Export.h"
#include <glm/glm.hpp>


class ENGINEDLL_API Entity {
public:
	Entity();
	~Entity() {}
	void GetPosition(float& x, float& y, float& z);
	void SetPosition(float x, float y, float z);
	void Translate(float x, float y, float z);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(float x, float y, float z);
	glm::mat4 GetModelMatrix() const;
private:
	void UpdateModel();
	void UpdateRotation();
	glm::mat4 m_model;
	glm::mat4 m_position;
	glm::mat4 m_rotation;
	glm::mat4 m_scale;

	glm::vec3 m_translate;
	glm::mat4 m_rotationX;
	glm::mat4 m_rotationY;
	glm::mat4 m_rotationZ;
};
