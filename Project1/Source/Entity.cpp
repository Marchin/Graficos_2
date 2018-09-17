#include "..\Headers\Entity.h"
#include <glm-0.9.9.0\glm\gtc\matrix_transform.hpp>

Entity::Entity() {
	m_position = glm::mat4(0.f);
	m_rotation = glm::mat4(0.f);
	m_scale = glm::mat4(1.f);
}

void Entity::SetPosition(float x, float y, float z){
	m_translate = glm::vec3(x, y, z);
	m_translate = glm::translate(glm::mat4(1.0f), m_position);
}

void Entity::RotateX(float angle){
	m_rotation.x = angle;
	m_model = glm::rotate(m_model)
}

void Entity::RotateY(float angle){
	m_rotation.y = angle;
}

void Entity::RotateZ(float angle){
	m_rotation.z = angle;
}

void Entity::Scale(float x, float y, float z){
	m_scale = glm::vec3(x, y, z);
}

glm::mat4 Entity::GetModelMatrix() const {
	return m_model;
}

glm::mat4 Entity::GetModel() const {
	return m_model;
}
