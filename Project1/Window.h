#pragma once
#include "Export.h"

class ENGINEDLL_API Window {
public:
	bool Start(unsigned int width, unsigned int height, const char* name);
	bool Stop();
	bool ShouldClose();
	void PollEvents();
	void Refresh();
	void* GetWindow();
private:
	void* m_pWindow;
	int m_width;
	int m_height;
	const char* m_pName;
};