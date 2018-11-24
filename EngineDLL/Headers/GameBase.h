#pragma once
#include "Export.h"
#include "Renderer.h"
#include "Window.h"

class ENGINEDLL_API GameBase {
public:
	virtual ~GameBase();
	bool Start();
	bool Stop();
	void Loop();
protected:
	GameBase();
	virtual bool OnStart() = 0;
	virtual bool OnStop() = 0;
	virtual bool OnUpdate() = 0;
	float deltaTime;
	float lastFrame;
	Renderer* m_pRenderer;
	Window* m_pWindow;
};