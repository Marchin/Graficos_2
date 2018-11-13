#pragma once
#include "Sprite.h"

struct Coords {
	float u0;
	float u1;
	float v0;
	float v1;
} typedef coords;

class ENGINEDLL_API SpriteSheet : public Sprite {
public:
	SpriteSheet(Renderer * pRenderer, Material material, const char* texturePath, void * position = NULL, void * uv = NULL);
	~SpriteSheet();
	void SetFrame(unsigned int frame);
	void SetFrameSize(unsigned int width, unsigned int height = 0);
	void SetFramesPerRow(unsigned int rows);
private:
	void SetupUV();
	unsigned int m_frameWidth;
	unsigned int m_frameHeight;
	unsigned int m_rows;
	unsigned int m_columns;
	coords* m_pUVCoords;
	float* m_pUVData;
};