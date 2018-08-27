#pragma once
#include "Export.h"

class ENGINEDLL_API Window {
public:
	Window(unsigned int width, unsigned int height, const char* name);
	bool Start();
	bool Stop();
private:
	void* m_window;
};