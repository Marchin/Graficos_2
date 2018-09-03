#pragma once

#include "Export.h"
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
private:

};
