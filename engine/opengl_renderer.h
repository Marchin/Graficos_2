#ifndef ENGINE_RENDER_H
#define ENGINE_RENDER_H

#define glCall(x) glClearError();\
x;\
Assert(glLogCall(#x, __FILE__, __LINE__));
#define UNIFORMS_MAX 32

struct ENGINE_API Shader {
    meow_hash hashLocationCache[UNIFORMS_MAX];
    i32 uniformLocationCache[UNIFORMS_MAX];
    u32 id;
};

struct ENGINE_API Texture {
    u32 id;
    i32 width;
    i32 height;
};

struct ElementBuffer {
    u32 ebObject;
    u32 count;
};

struct VertexBufferElement {
	u32 type;
	u32 count;
    u8 normalized;
};

struct VertexBufferLayout {
    VertexBufferElement* pElements;
    u32 elementsCount;
    u32 elementsMaxSize;
    u32 stride;
};

struct ENGINE_API Window {
    void* pInstance;
    i32 width;
    i32 height;
    char* pName;
};

enum Projection {
    ORTHOGONAL,
    PERSPECTIVE
};

struct ENGINE_API Renderer {
	hmm_mat4 model;
	hmm_mat4 view;
	hmm_mat4 projection;
	f32 camPosX;
	f32 camPosY;
	f32 halfCamWidth;
	f32 halfCamHeight;
    f32 fov;
    f32 aspectRatio;
    Projection projectionType;
};

ENGINE_API inline f32 getTime();
ENGINE_API void initShader(Shader* pShader,
                           char* pVertexPath, char* pFragmentPath,
                           char* pGeometryPath = 0, 
                           char* pTessControlPath = 0, char* pTessEvaluationPath = 0);
ENGINE_API inline void shaderBindID(u32 shaderID);
ENGINE_API i32 getUniformLocation(Shader* pShader, char* pName);
ENGINE_API inline void shaderSetBool(Shader* pShader, char* pName, b32 value);
ENGINE_API inline void shaderSetInt(Shader* pShader, char* pName, i32 value);
ENGINE_API inline void shaderSetFloat(Shader* pShader, char* pName, f32 value);
ENGINE_API inline void shaderSetVec3(Shader* pShader, char* pName, hmm_vec3* pVector);
ENGINE_API inline void shaderSetMat4(Shader* pShader, char* pName, hmm_mat4* pMat4);
ENGINE_API void initTexture(Texture* pTexture, u32 width, u32 height);
ENGINE_API void initTexture(Texture* pTexture,
                            char* pImgPath, 
                            b32 flipVertical = false, 
                            i32 TextureWrap_S = CLAMP_TO_EDGE, 
                            i32 TextureWrap_T = CLAMP_TO_EDGE,
                            i32 TextureMinFilter = LINEAR, 
                            i32 TextureMagFilter = LINEAR);
ENGINE_API inline void freeTexture(u32* pTextureID);
ENGINE_API inline void textureBindID(u32 textureID, u32 slot);
ENGINE_API inline void initEB(u32* pEBObject,  u32* pData, u32 count);
ENGINE_API inline void freeEB(u32* pEBObject);
ENGINE_API inline void ebBind(u32 ebObject);
ENGINE_API inline void ebUnbind();
ENGINE_API inline u32 vbElementGetSizeOfType(u32 type);
ENGINE_API void vbLayoutPushFloat(VertexBufferLayout* pVBLayout, u32 count);
ENGINE_API void vbLayoutPushUint(VertexBufferLayout* pVBLayout, u32 count);
ENGINE_API void vbLayoutPushUchar(VertexBufferLayout* pVBLayout, u32 count);
ENGINE_API inline void initVB(u32* pVB);
ENGINE_API inline void initVB(u32* pVB, void* pData, u32 size);
ENGINE_API inline void freeVB(u32* pVB);
ENGINE_API inline void vbBind(u32 vb);
ENGINE_API inline void vbUnbind();
ENGINE_API inline void vbSetData(u32 vb, void* pData, u32 size);
ENGINE_API inline void initVA(u32* pVA);
ENGINE_API inline void freeVA(u32* pVA);
ENGINE_API inline void vaBind(u32 va);
ENGINE_API inline void vaUnbind();
ENGINE_API void vaAddBuffer(u32 va, u32 vb, VertexBufferLayout* pLayout);
ENGINE_API void vaAddBufferByLocation(u32 va, u32 vb, 
                                      VertexBufferLayout* pLayout, u32 location);
ENGINE_API b32 startWindow(Window* pWindow);
ENGINE_API b32 stopWindow(Window* pWindow);
ENGINE_API inline b32 windowShouldClose(Window* pWindow);
ENGINE_API inline void pollEventsFromWindow(Window* pWindow);
ENGINE_API void updateProjection(Renderer* pRenderer);
ENGINE_API void setupRenderer(Renderer* pRenderer, Projection projection = ORTHOGONAL);
ENGINE_API inline b32 startRenderer();
ENGINE_API inline b32 stopRenderer();
ENGINE_API inline void clearRenderer();
ENGINE_API inline void fillColor(f32 red, f32 green, f32 blue);
ENGINE_API inline void swapBuffers(Window* pWindow);
ENGINE_API inline void drawBuffer(u32 offset, u32 count);
ENGINE_API inline void drawBufferStrip(u32 offset, u32 count);
ENGINE_API inline void drawBufferFan(u32 offset, u32 count);
ENGINE_API inline void drawElements(u32 count);
ENGINE_API inline void resetModelMatrix(Renderer* pRenderer);
ENGINE_API inline void multiplyModelMatrix(Renderer* pRenderer, hmm_mat4* pTransformation);
ENGINE_API inline void setModelMatrix(Renderer* pRenderer, hmm_mat4 model);
ENGINE_API inline hmm_mat4 getModelViewProj(Renderer* pRenderer);
ENGINE_API inline void getCameraPosition(Renderer* pRenderer, f32* pX, f32* pY);
ENGINE_API inline void setCameraPosition(Renderer* pRenderer, f32 x, f32 y);
ENGINE_API inline f32 getCameraWidth(Renderer* pRenderer);
ENGINE_API inline f32 getCameraHeight(Renderer* pRenderer);
#endif //ENGINE_RENDER_H