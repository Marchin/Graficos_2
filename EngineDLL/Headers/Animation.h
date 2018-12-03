#pragma once
#include "Export.h"
#include "SpriteSheet.h"

class ENGINEDLL_API Animation {
public:
	Animation(SpriteSheet* pSpriteSheet, const unsigned int* frames, unsigned int count);
	~Animation();
	void Update(float deltaTime);
	void SetFPS(unsigned int fps);
	void ChangeAnimation(const unsigned int* frames, unsigned int count);
private:
	SpriteSheet* m_pSpriteSheet;
	unsigned int* m_frames;
	unsigned int m_count;
	unsigned int m_currentFrame;
	float m_interval;
	float m_counter;
};
