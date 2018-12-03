#pragma once
#include "Shape.h"
#include "SpriteSheet.h"
#include "BoxCollider.h"
#define COLL_BITMASK_SIZE 4
#define MAX_COLLIDERS 64

class ENGINEDLL_API Tilemap : public Shape {
public:
	Tilemap(const char* fileDir, SpriteSheet* pSpriteSheet,
		Renderer* pRenderer, Material material);
	~Tilemap();
	virtual void Draw() override;
	void CheckCollisions();
	void SetCollisionableTiles(int* frames, int count);
	bool RegisterColliders(BoxCollider* collider);
private:
	void CalculateVertexPosition();
	void CalculateUV();
	void CalculateVisibleTiles();
	void WCoordsToColRow(float wcX, float wcY, int* col, int* row);
	void ColRowToWCoords(int col, int row, float* wcX, float* wcY);
	inline void CheckSideColliding(BoxCollider* collider, float offsetX, float offsetY);
	SpriteSheet m_spriteSheet;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_visibleWidth;
	unsigned int m_visibleHeight;
	unsigned int m_collidersCount;
	float m_tileSize;
	float m_cacheCamPosX;
	float m_cacheCamPosY;
	int* m_pIDs;
	int* m_pVisibleTiles;
	float* m_pUVData;
	float* m_pCoords;
	uint32_t m_collisionBitmask[COLL_BITMASK_SIZE] = {0};
	BoxCollider* m_colliders[MAX_COLLIDERS] = {NULL};
	Material m_material;
	Renderer* m_pRenderer;
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbUV;
};