#pragma once
#include "Headers\GameBase.h"
#include "Headers\Triangle.h"

class Game: public GameBase {
public:
	Game();
	~Game();
	
private:
	bool OnStart() override;
	bool OnStop() override;
	bool OnUpdate() override;
	int m_counter;
	Triangle* m_triangle;
};
