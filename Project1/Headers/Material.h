#pragma once
#include "Export.h"
#include "Shader.h"

class ENGINEDLL_API Material {
public:
	Material(Shader program);
	~Material() {}
	void Bind();
private:
	Shader m_program;
};