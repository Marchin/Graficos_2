#pragma once

#include "Export.h"
#include <glm/glm.hpp>
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class ENGINEDLL_API Renderer {
public:
	Renderer();
	~Renderer();
	bool Start();
	bool Stop();
	void Clear();
	void FillColor(float red, float green, float blue);
	void SwapBuffers(void* pWindow);
	void DrawBuffer(unsigned int offset, unsigned int count);
	void DrawBufferStrip(unsigned int offset, unsigned int count);
	void DrawBufferFan(unsigned int offset, unsigned int count);
	void DrawElements(unsigned int count);
	void ResetModelMatrix();
	void MultiplyModelMatrix(glm::mat4 matrix);
	void SetModelMatrix(glm::mat4 model);
	glm::mat4 GetModelViewProj() const;
private:
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
};
