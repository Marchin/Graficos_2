#ifndef TILEMAP_H
#define TILEMAP_H

#define COLL_BITMASK_SIZE 4
#define MAX_COLLIDERS 512

struct ENGINE_API Tilemap {
	s32 width;
	s32 height;
	s32 visibleWidth;
	s32 visibleHeight;
	u32 collidersCount;
	f32 tileSize;
	f32 cacheCamPosX;
	f32 cacheCamPosY;
	s32* pIDs;
	s32* pVisibleTiles;
	f32* pUVData;
	f32* pCoords;
    u32 va;
    u32 vbPosition;
    u32 vbUV;
    size_t collisionBitmask[COLL_BITMASK_SIZE] = {0};
	SpriteSheet tileset;
	BoxCollider* colliders[MAX_COLLIDERS] = {NULL};
    Shader material;
    Transform transform;
};

ENGINE_API inline b32 checkTilemapFormat(const char* pFileDir);
ENGINE_API b32 loadTilemap(Tilemap* pTilemap, const char* pFileDir);
ENGINE_API inline void tilemapWCoordsToColRow(Tilemap* pTilemap, 
                                              f32 wcX, f32 wcY, s32* pCol, s32* pRow);
ENGINE_API inline void tilemapColRowToWCoords(Tilemap* pTilemap, 
                                              u32 col, u32 row, f32* pWCX, f32* pWCY);
ENGINE_API void tilemapCheckSideColliding(Tilemap* pTilemap, BoxCollider* pCollider, 
                                          f32 offsetX, f32 offsetY);
ENGINE_API void tilemapCheckCollisions(Tilemap* pTilemap);
ENGINE_API inline void tilemapSetCollisionableTiles(Tilemap* pTilemap, 
                                                    s32* pFrames, s32 count);
ENGINE_API inline b32 tilemapRegisterColliders(Tilemap* pTilemap, BoxCollider* pCollider);
ENGINE_API void tilemapCalculateVertexPosition(Tilemap* pTilemap);
ENGINE_API void tilemapCalculateUV(Tilemap* pTilemap);
ENGINE_API void tilemapCalculateVisibleTiles(Tilemap* pTilemap, Renderer* pRenderer);
ENGINE_API void initTilemap(Tilemap* pTilemap, const char* pTilemapDir, 
                            SpriteSheet* pTileset, 
                            Shader* pMaterial, Renderer* pRenderer);
ENGINE_API inline void freeTilemap(Tilemap* pTilemap);
ENGINE_API void drawTilemap(Tilemap* pTilemap, Renderer* pRenderer);
#endif //TILEMAP_H