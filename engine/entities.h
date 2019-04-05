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
}; 

struct ENGINE_API Triangle {
    u32 va;
    u32 vb;
    Transform transform;
    Shader material;
}; 

struct ENGINE_API ColorSquare {
    u32 va;
    u32 vbPosition;
    u32 vbColor;
    Transform transform;
    Shader material;
};

struct ENGINE_API Circle {
    f32 radius;
    u32 sides;
    u32 va;
    u32 vb;
    Transform transform;
    Shader material;
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
    Shader material;
};

// NOTE(Marchin): see if there is a better way to architect this
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

ENGINE_API inline void transformUpdateMC(Transform* pTransform);
ENGINE_API void initTransform(Transform* pTransform);
ENGINE_API inline void transformSetPosition(Transform* pTransform, f32 x, f32 y, f32 z);
ENGINE_API inline void transformTranslate(Transform* pTransform, f32 x, f32 y, f32 z);
ENGINE_API inline void transformRotate(Transform* pTransform, f32 angle, hmm_vec3 axis);
ENGINE_API inline void transformScale(Transform* pTransform, f32 x, f32 y, f32 z);
ENGINE_API void initTriangle(Triangle* pTriangle, Shader* pMaterial, 
                             const void* pData, u32 size);
ENGINE_API void freeTriangle(Triangle* pTriangle);
ENGINE_API void drawTriangle(Triangle* pTriangle, Renderer* pRenderer);
ENGINE_API inline void setTriangleVertices(Triangle* pTriangle, const void* pData);
ENGINE_API void initColorSquare(ColorSquare* pCS, Shader* pMaterial, 
                                const void* pPosition, const void* pColor);
ENGINE_API void freeColorSquare(ColorSquare* pCS);
ENGINE_API void drawColorSquare(ColorSquare* pCS, Renderer* pRenderer);
ENGINE_API inline void colorSquareSetVertices(ColorSquare* pCS, const void* pPosition);
ENGINE_API inline void colorSquareSetColors(ColorSquare* pCS, const void* pColor);
ENGINE_API void circleRecalculate(Circle* pCircle);
ENGINE_API void initCircle(Circle* pCircle, Shader* pMaterial, u32 sidesAmount, f32 radius);
ENGINE_API inline void freeCircle(Circle* pCircle);
ENGINE_API void drawCircle(Circle* pCircle, Renderer* pRenderer);
ENGINE_API void initSpriteRenderer(SpriteRenderer* pSR, Shader* pMaterial, 
                                   const char* pTexturePath,
                                   const void* pPosition = 0, const void* pUV = 0);
ENGINE_API inline void spriteSetVertices(SpriteRenderer* pSR, const void* pPosition);
ENGINE_API inline void spriteSetUV(SpriteRenderer* pSR, const void* pUVCoords);
ENGINE_API void drawSpriteRenderer(SpriteRenderer* pSR, 
                                   const Transform* pTransform, 
                                   Renderer* pRenderer);
ENGINE_API void initSpriteSheet(SpriteSheet* pSS, Shader* pMaterial, 
                                const char* pTexturePath, 
                                const void* pPosition = 0, const void* pUV = 0);
ENGINE_API inline void freeSpriteSheet(SpriteSheet* pSS);
ENGINE_API void spriteSheetSetFrame(SpriteSheet* pSS, u32 frame);
ENGINE_API void spriteSheetSetupUV(SpriteSheet* pSS);
ENGINE_API void spriteSheetSetFrameSize(SpriteSheet* pSS, u32 width, u32 height = 0);
ENGINE_API void initAnimation(Animation* pAnimation, SpriteSheet* pSS, 
                              u32* pFrames, u32 count);
ENGINE_API inline void freeAnimation(Animation* pAnimation);
ENGINE_API void updateAnimation(Animation* pAnimation, f32 deltaTime);
ENGINE_API inline void setAnimationFPS(Animation* pAnimation, u32 fps);
ENGINE_API void changeAnimation(Animation* pAnimation, u32* pFrames, u32 count);
#endif //ENTITIES_H