#pragma once
#include "Shape.h"
#include "SpriteSheet.h"

class ENGINEDLL_API Tilemap: public Shape{
public:
	Tilemap(const char* fileDir, SpriteSheet* pSpriteSheet,
		Renderer* pRenderer, Material material);
	~Tilemap();
	virtual void Draw() override;

private:
	void CalculateVertexPosition();
	void CalculateUV();
	virtual void SetVertices(void* data) {}
	SpriteSheet m_spriteSheet;
	unsigned int m_width;
	unsigned int m_height;
	int* m_ids;
	float* m_pUVData;
	Material m_material;
	Renderer* m_pRenderer;
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbUV;
};