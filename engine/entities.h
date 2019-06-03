#ifndef ENTITIES_H
#define ENTITIES_H

struct ENGINE_API Transform {
    hmm_mat4 model;
	hmm_mat4 positionMatrix;
	hmm_mat4 rotationMatrix;
	hmm_mat4 scaleMatrix;
    
	hmm_vec3 position;
    hmm_vec3 eulerAngles;
    hmm_vec3 scale;
    
    void(*draw)();
    void(*update)();
    Transform* pParent;
    Transform* pChildren;
    u32 childrenCount;
}; 

struct ENGINE_API Triangle {
    u32 va;
    u32 vb;
    Transform transform;
    Material material;
}; 

struct ENGINE_API ColorSquare {
    u32 va;
    u32 vbPosition;
    u32 vbColor;
    Transform transform;
    Material material;
};

struct ENGINE_API Circle {
    f32 radius;
    u32 sides;
    u32 va;
    u32 vb;
    Transform transform;
    Material material;
};

struct ENGINE_API Coords {
	f32 u0;
	f32 u1;
	f32 v0;
	f32 v1;
};

struct ENGINE_API SpriteRenderer {
    u32 va;
    u32 vbPosition;
    u32 vbUV;
    Texture texture;
    Material material;
};

struct ENGINE_API SpriteSheet {
	u32 frameWidth;
	u32 frameHeight;
	u32 rows;
	u32 columns;
	f32* pUVData;
    Coords* pUVCoords;
    SpriteRenderer spriteRenderer;
};

struct ENGINE_API Animation {
	SpriteSheet* pSS;
	u32* pFrames;
	u32 count;
	u32 currentFrame;
	f32 interval;
	f32 counter;
};

struct ENGINE_API Vertex {
    hmm_vec3 pos;
    hmm_vec3 normal;
    hmm_vec2 uv;
};

struct ENGINE_API ModelTexture {
    meow_hash typeHash;
    char pPath[MAX_PATH_SIZE];
    u32 id;
};

global const u32 MAX_VERTICES = megabytes(128)/sizeof(Vertex);
global const u32 MAX_TEXTURES_POINTERS = kilobytes(512)/sizeof(ModelTexture);
global const u32 MAX_INDICES = megabytes(32)/sizeof(u32);
global const u32 MAX_IDS = megabytes(1);

enum MeshComponentType {
    VERTICES,
    TEXTURES,
    INDICES,
};

struct MeshComponentsPool {
    //Stacks of buffers
    Vertex vertices[MAX_VERTICES];
    ModelTexture* pTextures[MAX_TEXTURES_POINTERS];
    u32 indices[MAX_INDICES];
    
    //offset to end
    size_t verticesOffset;
    size_t texturesOffset;
    size_t indicesOffset;
    
    //directions of beginnings of buffers
    Vertex* verticesSlotsBeginnings[MAX_IDS]; // TODO(Marchin): choose a better name
    ModelTexture** texturesSlotsBeginnings[MAX_IDS]; // TODO(Marchin): choose a better name
    u32* indicesSlotsBeginnings[MAX_IDS]; // TODO(Marchin): choose a better name
    
    //bitmask, if a bit is set the id is currently in uses
    size_t idsUsed[MAX_IDS/sizeof(size_t)];
    
    //amount of ids in use
    size_t idsCount;
};

struct ENGINE_API Mesh {
    Material* pMaterial;
    Vertex* pVertices;
    ModelTexture** pModelTextures;
    u32* pIndices;
    size_t meshComponentID;
    u32 verticesCount;
    u32 texturesCount;
    u32 indicesCount;
    u32 va;
    u32 vb;
    u32 eb;
};

struct ENGINE_API Model {
    Transform transform;
    Material material;
    Mesh* pMeshes;
    ModelTexture* pLoadedTextures;
    char pPath[MAX_PATH_SIZE];
    u32 meshesCount;
    u32 texturesCount;
};

//TRANSFORM
ENGINE_API inline void transformUpdateMC(Transform* pTransform);
ENGINE_API void initTransform(Transform* pTransform);
ENGINE_API inline void transformSetPosition(Transform* pTransform, f32 x, f32 y, f32 z);
ENGINE_API inline void transformTranslate(Transform* pTransform, f32 x, f32 y, f32 z);
ENGINE_API inline void transformRotate(Transform* pTransform, f32 angle, hmm_vec3 axis);
ENGINE_API inline void transformScale(Transform* pTransform, f32 x, f32 y, f32 z);

//TRIANGLE
ENGINE_API void initTriangle(Triangle* pTriangle, Material* pMaterial, 
                             const void* pData, u32 size);
ENGINE_API void freeTriangle(Triangle* pTriangle);
ENGINE_API void drawTriangle(Triangle* pTriangle, Renderer* pRenderer);
ENGINE_API inline void setTriangleVertices(Triangle* pTriangle, const void* pData);

//COLOR_SQUARE
ENGINE_API void initColorSquare(ColorSquare* pCS, Material* pMaterial, 
                                const void* pPosition, const void* pColor);
ENGINE_API void freeColorSquare(ColorSquare* pCS);
ENGINE_API void drawColorSquare(ColorSquare* pCS, Renderer* pRenderer);
ENGINE_API inline void colorSquareSetVertices(ColorSquare* pCS, const void* pPosition);
ENGINE_API inline void colorSquareSetColors(ColorSquare* pCS, const void* pColor);

//CIRCLE
ENGINE_API void circleRecalculate(Circle* pCircle);
ENGINE_API void initCircle(Circle* pCircle, Material* pMaterial, u32 sidesAmount, f32 radius);
ENGINE_API inline void freeCircle(Circle* pCircle);
ENGINE_API void drawCircle(Circle* pCircle, Renderer* pRenderer);

//SPRITE_RENDERER
ENGINE_API void initSpriteRenderer(SpriteRenderer* pSR, Material* pMaterial, 
                                   const char* pTexturePath,
                                   const void* pPosition = 0, const void* pUV = 0);
ENGINE_API inline void spriteSetVertices(SpriteRenderer* pSR, const void* pPosition);
ENGINE_API inline void spriteSetUV(SpriteRenderer* pSR, const void* pUVCoords);
ENGINE_API void drawSpriteRenderer(SpriteRenderer* pSR, 
                                   const Transform* pTransform, 
                                   Renderer* pRenderer);

//SPRITE_SHEET
ENGINE_API void initSpriteSheet(SpriteSheet* pSS, Material* pMaterial, 
                                const char* pTexturePath, 
                                const void* pPosition = 0, const void* pUV = 0);
ENGINE_API inline void freeSpriteSheet(SpriteSheet* pSS);
ENGINE_API void spriteSheetSetFrame(SpriteSheet* pSS, u32 frame);
ENGINE_API void spriteSheetSetFrameSize(SpriteSheet* pSS, u32 width, u32 height = 0);

//ANIMATION
ENGINE_API void initAnimation(Animation* pAnimation, SpriteSheet* pSS, 
                              u32* pFrames, u32 count);
ENGINE_API inline void freeAnimation(Animation* pAnimation);
ENGINE_API void updateAnimation(Animation* pAnimation, f32 deltaTime);
ENGINE_API inline void setAnimationFPS(Animation* pAnimation, u32 fps);
ENGINE_API void changeAnimation(Animation* pAnimation, u32* pFrames, u32 count);

//MODEL
ENGINE_API void initMesh(Mesh* pMesh);
ENGINE_API void drawMesh(Mesh* pMesh);
ENGINE_API void freeMesh(Mesh* pMesh);
ENGINE_API void drawModel(Model* pModel, Renderer* pRenderer);
ENGINE_API u32 textureFromFile(const char* pTextureName, const char* pModelPath);
ENGINE_API void initModel(Model* pModel, const char* pPath, const Material* pMaterial);
ENGINE_API void freeModel(Model* pModel);
#endif //ENTITIES_H