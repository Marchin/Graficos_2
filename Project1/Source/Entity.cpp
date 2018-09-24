#include "..\Headers\Entity.h"
#include <glm\gtc\matrix_transform.hpp>

Entity::Entity() {
	m_position = glm::mat4(0.f);
	m_rotation = glm::mat4(1.f);
	m_scale = glm::mat4(1.f);
	UpdateModel();
}

void Entity::SetPosition(float x, float y, float z){
	m_translate = glm::vec3(x, y, z);
	m_position = glm::translate(glm::mat4(1.0f), m_translate);
	UpdateModel();
}

void Entity::Translate(float x, float y, float z) {
	m_translate += glm::vec3(x, y, z);
	m_position = glm::translate(glm::mat4(1.0f), m_translate);
	UpdateModel();
}

void Entity::RotateX(float angle){
	glm::vec3 axis(1.f, 0.f, 0.f);
	m_rotationX = glm::rotate(m_model, angle, axis);
	UpdateRotation();
	UpdateModel();
}

void Entity::RotateY(float angle){
	glm::vec3 axis(0.f, 1.f, 0.f);
	m_rotationY = glm::rotate(m_model, angle, axis);
	UpdateRotation();
	UpdateModel();
}

void Entity::RotateZ(float angle){
	glm::vec3 axis(0.f, 1.f, 0.f);
	m_rotationZ = glm::rotate(m_model, angle, axis);
	UpdateRotation();
	UpdateModel();
}

void Entity::Scale(float x, float y, float z){
	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
	UpdateModel();
}

glm::mat4 Entity::GetModelMatrix() const {
	return m_model;
}

void Entity::UpdateModel() {
	m_model = glm::mat4(m_position + (m_rotation *  m_scale));
}

void Entity::UpdateRotation() {
	m_rotation = m_rotationX * m_rotationY * m_rotationZ;
}
