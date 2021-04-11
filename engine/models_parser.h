#ifndef MODELS_PARSER_H

struct Face {
    u32 vertexIndices[3];
    u32 uvIndices[3];
};

struct ModelData {
    Component component;
    
    V3* pVertices;
    u32 vertexCount;
    
    V2* pUV;
    u32 uvCount;
    
    Face* pFaces;
    u32 facesCount;
    
    u32 va;
    u32 vb;
    
    Texture texture;
    
    Shader shader;
    Transform transform;
};

ENGINE_API void
parseModel(ModelData* pModelData, char* path);

ENGINE_API void
initParsedModel(ModelData* pModelData, Shader* pShader);

ENGINE_API b32
drawParsedModel(void* pModelData, Renderer* pRenderer);

#define MODELS_PARSER_H
#endif //MODELS_PARSER_H
