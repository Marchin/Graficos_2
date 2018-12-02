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
	void CalculateVisibleTiles();
	SpriteSheet m_spriteSheet;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_visibleWidth;
	unsigned int m_visibleHeight;
	float m_tileSize;
	float m_cacheCamPosX;
	float m_cacheCamPosY;
	int* m_pIDs;
	int* m_pVisibleTiles;
	float* m_pUVData;
	float* m_pCoords;
	Material m_material;
	Renderer* m_pRenderer;
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbUV;
};