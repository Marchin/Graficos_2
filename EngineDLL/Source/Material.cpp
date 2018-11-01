#include "../Headers/Material.h"

Material::Material(Shader program) 
	: m_program(program) {}

void Material::Bind() {
	m_program.Bind();
}

void Material::SetIntProperty(const char* name, int value) {
	m_program.SetInt(name, value);
}

void Material::SetMatrixProperty(const char* name, glm::mat4 mat) {
	m_program.SetMat4(name, mat);
}
