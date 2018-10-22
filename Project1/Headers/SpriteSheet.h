#pragma once
#include "Sprite.h"

class ENGINEDLL_API SpriteSheet : public Sprite {
public:
	SpriteSheet(Renderer * pRenderer, Material material, const char* texturePath, void * position, void * uv);
	void SetFrameSize(unsigned int width, unsigned int height = 0);
	void SetFramesPerRow(unsigned int amount);
private:
	void SetupUV();
	unsigned int m_frameWidth;
	unsigned int m_frameHeight;
	unsigned int m_rows;
	unsigned int m_columns;
	unsigned int m_framesPerRow;
	unsigned int m_framesPerColumn;
	float* m_uvCoords;
};