#pragma once
#include "Export.h"

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
