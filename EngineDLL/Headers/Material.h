#pragma once
#include "Export.h"
#include "Shader.h"
#include "TextureController.h"
#include <glm/glm.hpp>


class ENGINEDLL_API Material {
public:
	Material(Shader program);
	~Material() {}
	void Bind();
	void SetIntProperty(const char* name, int value);
	void SetUintProperty(const char* name, int value);
	void SetFloatProperty(const char* name, float value);
	void SetMatrixProperty(const char* name, glm::mat4 mat);
private:
	Shader m_program;
};