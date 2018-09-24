#pragma once
#include "Export.h"
#include "Shader.h"
#include <glm/glm.hpp>


class ENGINEDLL_API Material {
public:
	Material(Shader program);
	~Material() {}
	void Bind();
	void SetMatrixProperty(const char* name, glm::mat4 mat);
private:
	Shader m_program;
};