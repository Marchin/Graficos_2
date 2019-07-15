////////////////////////////////

//Components

////////////////////////////////

internal Component*
createComponent(ComponentID componentID) {
    Component* result = {};
    switch (componentID) {
        case TRIANGLE: {
            result = (Component*)malloc(sizeof(Triangle));
            memset(result, 0, sizeof(Triangle));
        } break;
        case COLOR_SQUARE: {
            result = (Component*)malloc(sizeof(ColorSquare));
            memset(result, 0, sizeof(ColorSquare));
        } break;
        case CIRCLE: {
            result = (Component*)malloc(sizeof(Circle));
            memset(result, 0, sizeof(Circle));
        } break;
        case SPRITE_RENDERER: {
            result = (Component*)malloc(sizeof(SpriteRenderer));
            memset(result, 0, sizeof(SpriteRenderer));
        } break;
        case SPRITE_SHEET: {
            result = (Component*)malloc(sizeof(SpriteSheet));
            memset(result, 0, sizeof(SpriteSheet));
        } break;
        case ANIMATION: {
            result = (Component*)malloc(sizeof(Animation));
            memset(result, 0, sizeof(Animation));
        } break;
        case MODEL: {
            result = (Component*)malloc(sizeof(Model));
            memset(result, 0, sizeof(Model));
        } break;
        default: {
            result = 0;
        } break;
    }
    
    return result;
}

ENGINE_API inline Component*
getComponent(ComponentID componentID, Component** pComponents, s32 componentsSize) {
    for (s32 i = 0; i < componentsSize; ++i) {
        if (pComponents[i]->id == componentID) {
            return pComponents[i];
        }
    }
    
    return 0;
}

ENGINE_API inline Component*
addComponent(ComponentID componentID, Transform* pTransform) {
    u32 componentsCapacity = pTransform->componentsCapacity;
    if (pTransform->componentsCount == componentsCapacity) {
        componentsCapacity += DEFAULT_CHILDREN_ADDED;
        
        pTransform->pComponents = (Component**)realloc(pTransform->pComponents, 
                                                       componentsCapacity * sizeof(Component*));
        memset(pTransform->pComponents, 0, componentsCapacity * sizeof(Component*));
        pTransform->componentsCapacity = componentsCapacity;
    }
    for (u32 i = 0; i < componentsCapacity; ++i) {
        if (pTransform->pComponents[i] == NULL) {
            pTransform->pComponents[i] = createComponent(componentID);
            ++pTransform->componentsCount;
            return pTransform->pComponents[i];
        }
    }
    
    return 0;
}

ENGINE_API inline void
removeComponent(ComponentID componentID, Component** pComponents, s32 componentsSize) {
    for (s32 i = 0; i < componentsSize; ++i) {
        if (pComponents[i] && pComponents[i]->id == componentID) {
            free(pComponents[i]);
            pComponents[i] = 0;
        }
    }
}

////////////////////////////////

//Transform

////////////////////////////////

ENGINE_API inline void 
transformUpdateMC(Transform* pTransform) {
    pTransform->model = 
        pTransform->positionMatrix * pTransform->rotationMatrix * pTransform->scaleMatrix;
}

ENGINE_API void
initTransform(Transform* pTransform) {
    *pTransform = {};
    pTransform->positionMatrix = HMM_Mat4d(1.f);
    pTransform->rotationMatrix = HMM_Mat4d(1.f);
    pTransform->scaleMatrix = HMM_Mat4d(1.f);
    pTransform->position = HMM_Vec3T(0.f);
    pTransform->eulerAngles = HMM_Vec3T(0.f);
    pTransform->scale = HMM_Vec3T(1.f);
    
    transformUpdateMC(pTransform);
}

ENGINE_API inline void
reserveChildren(Transform* pTransform, u32 amount) {
    pTransform->maxAmountOfChildren += amount;
    pTransform->pChildren = (Transform**)realloc(pTransform->pChildren, 
                                                 pTransform->maxAmountOfChildren * sizeof(Transform*));
}

ENGINE_API inline void
addChild(Transform* pChild, Transform* pParent) {
    u32 maxAmountOfChildren = pParent->maxAmountOfChildren;
    if (pParent->childrenCount == maxAmountOfChildren) {
        maxAmountOfChildren += DEFAULT_CHILDREN_ADDED;
        pParent->pChildren = (Transform**)realloc(pParent->pChildren, 
                                                  maxAmountOfChildren * sizeof(Transform*));
        pParent->maxAmountOfChildren = maxAmountOfChildren;
    }
    pParent->pChildren[pParent->childrenCount++] = pChild;
    pChild->pParent = pParent;
}

ENGINE_API inline void
removeChild(Transform* pChild) {
    Transform* pParent = pChild->pParent;
    
    if (pParent) {
        u32 childrenCount = pParent->childrenCount;
        for (u32 iTransform = 0; iTransform < childrenCount; ++iTransform) {
            if (pParent->pChildren[iTransform] == pChild) {
                pParent->pChildren[iTransform] = 0;
                --pParent->childrenCount;
                pChild->pParent = 0;
                break;
            }
        }
    }
}

ENGINE_API inline void 
transformSetPosition(Transform* pTransform, f32 x, f32 y, f32 z) {
    pTransform->position = HMM_Vec3(x, y, z);
    pTransform->positionMatrix = HMM_Translate(pTransform->position);
    transformUpdateMC(pTransform);
}

ENGINE_API inline void 
transformTranslate(Transform* pTransform, f32 x, f32 y, f32 z) {
    pTransform->position += HMM_Vec3(x, y, z);
    pTransform->positionMatrix = HMM_Translate(pTransform->position);
    transformUpdateMC(pTransform);
}

ENGINE_API inline void 
transformRotate(Transform* pTransform, f32 angle, hmm_vec3 axis) {
    pTransform->eulerAngles += angle * axis;
    pTransform->eulerAngles %= 360.f;
    pTransform->rotationMatrix = HMM_Rotate(angle, axis) * pTransform->rotationMatrix;
    transformUpdateMC(pTransform);
}

ENGINE_API inline void
transformScale(Transform* pTransform, f32 x, f32 y, f32 z) {
    pTransform->scale = HMM_Vec3(x, y, z);
    pTransform->scaleMatrix = HMM_Scale(pTransform->scale);
    transformUpdateMC(pTransform);
}

ENGINE_API inline void
transformDraw(Transform* pTransform, Renderer* pRenderer) {
    hmm_mat4 modelMatrix = pTransform->model;
    u32 childrenCount = pTransform->childrenCount;
    for (u32 iTransform = 0, i = 0; iTransform < childrenCount; ++iTransform){
        Transform* pChild = pTransform->pChildren[iTransform];
        if (pChild == 0) { continue; }
        hmm_mat4 modelChild = pChild->model;
        pChild->model = pTransform->model * pChild->model;
        if (pChild->draw) {
            pChild->draw(pChild->pEntity, pRenderer);
        }
        u32 componentCount = pChild->componentsCount;
        for (u32 iComponent = 0; iComponent < componentCount; ++iComponent) {
            if (pChild->pComponents[iComponent] && pChild->pComponents[iComponent]->draw) {
                pChild->pComponents[iComponent]->draw(pChild->pComponents[iComponent], pRenderer);
            }
        }
        transformDraw(pChild, pRenderer);
        pChild->model = modelChild;
        ++i;
    }
}

ENGINE_API inline void
transformUpdate(Transform* pTransform, f32 deltaTime) {
    u32 childrenCount = pTransform->childrenCount;
    for (u32 iTransform = 0, i = 0; i < childrenCount; ++iTransform){
        Transform* pChild = pTransform->pChildren[iTransform];
        if (pChild == 0) { continue; }
        if (pChild->update) {
            pChild->update(pChild->pEntity, deltaTime);
        }
        transformUpdate(pChild, deltaTime);
        ++i;
    }
}

////////////////////////////////

//TRIANGLE

////////////////////////////////

ENGINE_API void
initTriangle(Triangle* pTriangle, 
             Transform* pTransform, Material* pMaterial, 
             const void* pData, u32 size) {
    pTriangle->component.id = TRIANGLE;
    pTriangle->component.draw = drawTriangle;
    initVA(&pTriangle->va);
    vaBind(pTriangle->va);
    initVB(&pTriangle->vb, pData, size);
    pTriangle->pTransform = pTransform;
    pTriangle->pMaterial = pMaterial;
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 1;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    vaAddBuffer(pTriangle->va, pTriangle->vb, &layout);
    free(layout.pElements);
}

ENGINE_API void
freeTriangle(Triangle* pTriangle) {
    freeVA(&pTriangle->va);
    freeVB(&pTriangle->vb);
}

ENGINE_API void
drawTriangle(void* pTriangle, Renderer* pRenderer) {
    Triangle* pCastedTriangle = (Triangle*) pTriangle; 
    materialBindID(pCastedTriangle->pMaterial->id);
    pRenderer->pCamera->model = pCastedTriangle->pTransform->model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(pCastedTriangle->pMaterial, 
                  "uModelViewProjection", 
                  &mvp);
    vaBind(pCastedTriangle->va);
    drawBufferStrip(0, 3);
}

ENGINE_API inline void 
setTriangleVertices(Triangle* pTriangle, const void* pData) {
    vbSetData(pTriangle->vb, pData, 12 * sizeof(f32));
}


////////////////////////////////

//COLOR_SQUARE

////////////////////////////////

ENGINE_API void
initColorSquare(ColorSquare* pCS, 
                Transform* pTransform, Material* pMaterial, 
                const void* pPosition, const void* pColor) {
    
    pCS->component.id = COLOR_SQUARE;
    pCS->component.draw = drawColorSquare;
    initVA(&pCS->va);
    vaBind(pCS->va);
    initVB(&pCS->vbPosition, pPosition, 12 * sizeof(f32));
    initVB(&pCS->vbColor, pColor, 12 * sizeof(f32));
    pCS->pTransform = pTransform;
    pCS->pMaterial = pMaterial;
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 2;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    vbLayoutPushFloat(&layout, 3);
    vaAddBufferByLocation(pCS->va, pCS->vbPosition, &layout, 0);
    vaAddBufferByLocation(pCS->va, pCS->vbColor, &layout, 1);
    free(layout.pElements);
}

ENGINE_API void
freeColorSquare(ColorSquare* pCS) {
    freeVA(&pCS->va);
    freeVB(&pCS->vbPosition);
    freeVB(&pCS->vbColor);
}

ENGINE_API void
drawColorSquare(void* pCS, Renderer* pRenderer) {
    ColorSquare* pCastedCS = (ColorSquare*)pCS;
    materialBindID(pCastedCS->pMaterial->id); 
    pRenderer->pCamera->model = pCastedCS->pTransform->model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(pCastedCS->pMaterial, "uModelViewProjection", &mvp);
    vaBind(pCastedCS->va);
    drawBufferStrip(0, 4);
}

ENGINE_API inline void
colorSquareSetVertices(ColorSquare* pCS, const void* pPosition) {
    vbSetData(pCS->vbPosition, pPosition, 12 * sizeof(f32));
}

ENGINE_API inline void
colorSquareSetColors(ColorSquare* pCS, void * pColor) {
    vbSetData(pCS->vbColor, pColor, 12 * sizeof(f32));
}


////////////////////////////////

//CIRCLE

////////////////////////////////

ENGINE_API void
circleRecalculate(Circle* pCircle) {
    if (pCircle->sides < 3) {
        pCircle->sides = 3;
    } 
    u32 sides = pCircle->sides;
    f32 currAngle = 0.f;
    f32 rate = 2.f * 3.1415f / (f32)sides;
    f32* pData = (f32*)malloc(sizeof(f32) * ((2 + sides) * 3));
    
    pData[0] = 0.f;
    pData[1] = 0.f;
    pData[2] = 0.f;
    
    pData[3] = 1.f * pCircle->radius;
    pData[4] = 0.f;
    pData[5] = 0.f;
    
    f32 radius = pCircle->radius;
    for (u32  i = 0; i < sides; ++i) {
        currAngle += rate;
        pData[6 + (i * 3)] = cosf(currAngle) * radius;
        pData[7 + (i * 3)] = sinf(currAngle) * radius;
        pData[8 + (i * 3)] = 0.f;
    }
    
    vbSetData(pCircle->vb, pData, sizeof(f32) * ((2 + pCircle->sides) * 3));
    free(pData);
}

ENGINE_API void
initCircle(Circle* pCircle, 
           Transform* pTransform, Material* pMaterial, 
           u32 sidesAmount, f32 radius) {
    pCircle->sides = sidesAmount;
    pCircle->radius = radius;
    
    pCircle->component.id = CIRCLE;
    pCircle->component.draw = drawCircle;
    initVA(&pCircle->va);
    vaBind(pCircle->va);
    initVB(&pCircle->vb);
    pCircle->pTransform = pTransform;
    pCircle->pMaterial = pMaterial;
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 1;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    circleRecalculate(pCircle);
    vaAddBuffer(pCircle->va, pCircle->vb, &layout);
    free(layout.pElements);
}

ENGINE_API inline void
freeCircle(Circle* pCircle) {
    freeVA(&pCircle->va);
    freeVB(&pCircle->vb);
}

ENGINE_API void
drawCircle(void* pCircle, Renderer* pRenderer) {
    Circle* pCastedCircle = (Circle*)pCircle;
    materialBindID(pCastedCircle->pMaterial->id);
    pRenderer->pCamera->model = pCastedCircle->pTransform->model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(pCastedCircle->pMaterial, "uModelViewProjection", &mvp);
    vaBind(pCastedCircle->va);
    drawBufferFan(0, pCastedCircle->sides + 2);
}


////////////////////////////////

//SPRITE_RENDERER

////////////////////////////////

ENGINE_API void
initSpriteRenderer(SpriteRenderer* pSR, 
                   Transform* pTransform, Material* pMaterial, 
                   const char* pTexturePath,
                   const void* pPosition, const void* pUV) {
    
    pSR->component.id = SPRITE_RENDERER;
    pSR->component.draw = drawSpriteRenderer;
    if (pPosition == NULL) {
        f32 squareVertices[] = {
            -1.f, -1.f, 0.f,
            -1.f,  1.f, 0.f,
            1.f, -1.f, 0.f,
            1.f,  1.f, 0.f
        };
    }
    if (pUV == NULL) {
        f32 squareUV[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 0.f,
            1.f, 1.f
        };
    }
    
    initTexture(&pSR->texture, pTexturePath, true);
    initVA(&pSR->va);
    vaBind(pSR->va);
    initVB(&pSR->vbPosition, pPosition, 12 * sizeof(f32));
    initVB(&pSR->vbUV, pUV, 8 * sizeof(f32));
    pSR->pMaterial = pMaterial;
    pSR->pTransform = pTransform;
    materialBindID(pSR->pMaterial->id);
    textureBindID(pSR->texture.id, 0);
    shaderSetInt(pSR->pMaterial, "tex", 0);
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 1;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    
    VertexBufferLayout layout2 = {};
    layoutsAmount = 1;
    layout2.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout2.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout2.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout2, 2);
    
    vaAddBufferByLocation(pSR->va, pSR->vbPosition, &layout, 0);
    vaAddBufferByLocation(pSR->va, pSR->vbUV, &layout2, 1);
    free(layout.pElements);
    free(layout2.pElements);
}

ENGINE_API inline void
spriteSetVertices(SpriteRenderer* pSR, const void* pPosition) {
    vbSetData(pSR->vbPosition, pPosition, 12 * sizeof(f32));
}

ENGINE_API inline void
spriteSetUV(SpriteRenderer* pSR, const void* pUVCoords) {
    vbSetData(pSR->vbUV, pUVCoords, 8 * sizeof(f32));
}

ENGINE_API void
drawSpriteRenderer(void* pSR, Renderer* pRenderer) {
    SpriteRenderer* pCastedSR = (SpriteRenderer*)pSR;
    materialBindID(pCastedSR->pMaterial->id);
    textureBindID(pCastedSR->texture.id, 0);
    pRenderer->pCamera->model = pCastedSR->pTransform->model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(pCastedSR->pMaterial, "uModelViewProjection", &mvp);
    vaBind(pCastedSR->va);
    drawBufferStrip(0, 4);
}


////////////////////////////////

//SPRITE_SHEET

////////////////////////////////

ENGINE_API void
initSpriteSheet(SpriteSheet* pSS, 
                Transform* pTransform, Material* pMaterial, 
                const char* pTexturePath, 
                const void* pPosition, const void* pUV) { 
    
    *pSS = {};
    pSS->component.id = SPRITE_SHEET;
    initSpriteRenderer(&pSS->spriteRenderer, 
                       pTransform, pMaterial, 
                       pTexturePath, 
                       pPosition, pUV);
}

ENGINE_API inline void
freeSpriteSheet(SpriteSheet* pSS) {
    free(pSS->pUVCoords);
    free(pSS->pUVData);
}

ENGINE_API void
spriteSheetSetFrame(SpriteSheet* pSS, u32 frame) {
    f32 buff[8] = {
        pSS->pUVData[(frame * 8) + 0], pSS->pUVData[(frame * 8) + 1],
        pSS->pUVData[(frame * 8) + 2], pSS->pUVData[(frame * 8) + 3],
        pSS->pUVData[(frame * 8) + 4], pSS->pUVData[(frame * 8) + 5],
        pSS->pUVData[(frame * 8) + 6], pSS->pUVData[(frame * 8) + 7]
    };
    vbSetData(pSS->spriteRenderer.vbUV, buff, sizeof(buff));
}

internal void
spriteSheetSetupUV(SpriteSheet* pSS) {
    if (pSS->columns == 0) {
        assert(pSS->frameWidth != 0);
        pSS->columns = pSS->spriteRenderer.texture.width / pSS->frameWidth;
    }
    if (pSS->rows == 0) {
        assert(pSS->frameHeight != 0);
        pSS->rows = pSS->spriteRenderer.texture.height / pSS->frameHeight;
    }
    if (pSS->pUVCoords != 0) {
        free(pSS->pUVCoords);
        pSS->pUVCoords = 0;
    }
    pSS->pUVCoords = (Coords*)malloc(sizeof(Coords) *(pSS->columns * pSS->rows));
    s32 id = 0;
    s32 ssWidth = pSS->spriteRenderer.texture.width;
    s32 ssHeight = pSS->spriteRenderer.texture.height;
    for (u32 iRow = 0; iRow < pSS->rows; ++iRow) {
        for (u32 iColumn = 0; iColumn < pSS->columns; ++iColumn) {
            f32 x = (f32)(id % pSS->columns) * (f32)pSS->frameWidth;
            f32 y = (f32)(id / pSS->columns) * (f32)pSS->frameHeight;
            pSS->pUVCoords[id].u0 = x / (f32)ssWidth;
            pSS->pUVCoords[id].u1 = 
                (x + (f32)pSS->frameWidth) / (f32)ssWidth;
            pSS->pUVCoords[id].v0 = 1.f - (y / (f32)ssHeight) ;
            pSS->pUVCoords[id].v1 = 
                1.f - ((y + (f32)pSS->frameHeight) / (f32)ssHeight); 
            
            ++id;
        }
    }
    if (pSS->pUVData != 0) {
        delete pSS->pUVCoords;
        pSS->pUVData = 0;
    }
    pSS->pUVData = (f32*)malloc(sizeof(f32) * id * 8);
    for (s32 i = 0; i < id; ++i) {
        pSS->pUVData[i * 8] = pSS->pUVCoords[i].u0;
        pSS->pUVData[i * 8 + 1] = pSS->pUVCoords[i].v1;
        pSS->pUVData[i * 8 + 2] = pSS->pUVCoords[i].u0;
        pSS->pUVData[i * 8 + 3] = pSS->pUVCoords[i].v0;
        pSS->pUVData[i * 8 + 4] = pSS->pUVCoords[i].u1;
        pSS->pUVData[i * 8 + 5] = pSS->pUVCoords[i].v1;
        pSS->pUVData[i * 8 + 6] = pSS->pUVCoords[i].u1;
        pSS->pUVData[i * 8 + 7] = pSS->pUVCoords[i].v0;
    }
    spriteSheetSetFrame(pSS, 0);
}

ENGINE_API void
spriteSheetSetFrameSize(SpriteSheet* pSS, u32 width, u32 height) {
    pSS->frameWidth = width;
    if (height == 0) {
        pSS->frameHeight = pSS->frameWidth;
    } else {
        pSS->frameHeight = height;
    }
    spriteSheetSetupUV(pSS);
}


////////////////////////////////

//ANIMATION

////////////////////////////////

ENGINE_API void
initAnimation(Animation* pAnimation, SpriteSheet* pSS, u32* pFrames, u32 count) {
    *pAnimation = {};
    pAnimation->component.id = ANIMATION;
    pAnimation->component.update = updateAnimation;
    pAnimation->pSS = pSS;
    pAnimation->interval = 1.f / 3.f;
    pAnimation->count = count;
    pAnimation->pFrames = (u32*)malloc(count * sizeof(u32));
    memcpy(pAnimation->pFrames, pFrames, count * sizeof(u32));
}

ENGINE_API inline void
freeAnimation(Animation* pAnimation) {
    free(pAnimation->pFrames);
}

ENGINE_API void
updateAnimation(void* pAnimation, f32 deltaTime) {
    Animation* pCastedAnimation = (Animation*)pAnimation;
    pCastedAnimation->counter += deltaTime;
    if (pCastedAnimation->counter >= pCastedAnimation->interval) {
        pCastedAnimation->currentFrame++;
        pCastedAnimation->currentFrame %= pCastedAnimation->count;
        spriteSheetSetFrame(pCastedAnimation->pSS,
                            *(pCastedAnimation->pFrames + pCastedAnimation->currentFrame));
        pCastedAnimation->counter = 0.f;
    }
}

ENGINE_API inline void
setAnimationFPS(Animation* pAnimation, u32 fps) {
    pAnimation->interval = 1.f / (f32)fps;
}

ENGINE_API void
changeAnimation(Animation* pAnimation, u32* pFrames, u32 count) {
    pAnimation->currentFrame = 0;
    pAnimation->counter = 0.f;
    pAnimation->count = count;
    free(pAnimation->pFrames);
    pAnimation->pFrames = (u32*)malloc(count * sizeof(u32));
    memcpy(pAnimation->pFrames, pFrames, count * sizeof(u32));
}


////////////////////////////////

//Models

////////////////////////////////


internal size_t
getMeshComponentID() {
    u32 size = sizeof(size_t);
    for (size_t iID = 0; iID < MAX_IDS; ++iID) {
        size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
        size_t offset = iID%size;
        if (((((*pCurrentWord  >> offset)) & 1) == 0)) {
            *pCurrentWord = *pCurrentWord | ((size_t)1 << offset);
            
            ++gpMeshComponentsPool->idsCount;
            return iID;
        }
    }
    assert(0);
    return 0;
}

internal void
pushToStack(size_t id, MeshComponentType type, u32 amountToReserve) {
    u8* pBuffer; 
    size_t* pOffset;
    size_t bufferSize;
    size_t sizeOfType;
    
    if (type == VERTICES) {
        pBuffer = (u8*)gpMeshComponentsPool->vertices;
        pOffset = &gpMeshComponentsPool->verticesOffset;
        bufferSize = MAX_VERTICES;
        sizeOfType = sizeof(Vertex);
        
        if (gpMeshComponentsPool->verticesSlotsBeginnings[id] != 0) {
            return;
        }
    } else if (type == TEXTURES) {
        pBuffer = (u8*)gpMeshComponentsPool->pTextures;
        pOffset = &gpMeshComponentsPool->texturesOffset;
        bufferSize = MAX_TEXTURES_POINTERS;
        sizeOfType = sizeof(ModelTexture*);
        
        if (gpMeshComponentsPool->texturesSlotsBeginnings[id] != 0) {
            return;
        }
    } else if (type == INDICES) {
        pBuffer = (u8*)gpMeshComponentsPool->indices;
        pOffset = &gpMeshComponentsPool->indicesOffset;
        bufferSize = MAX_INDICES;
        sizeOfType = sizeof(u32);
        
        if (gpMeshComponentsPool->indicesSlotsBeginnings[id] != 0) {
            return;
        }
    } else {
        return;
    }
    
    u8* pEndOfStack = pBuffer + (*pOffset)*sizeOfType;
    void* pReserved = 0;
    
    if (pEndOfStack + amountToReserve*sizeOfType < pBuffer + bufferSize*sizeOfType) {
        pReserved = pEndOfStack;
        *pOffset += amountToReserve;
    } else {
        assert(false);
    }
    
    if (type == VERTICES) {
        gpMeshComponentsPool->verticesSlotsBeginnings[id] = (Vertex*)pReserved;
    }
    else if (type == TEXTURES) {
        gpMeshComponentsPool->texturesSlotsBeginnings[id] = (ModelTexture**)pReserved;
    }
    else if (type == INDICES) {
        gpMeshComponentsPool->indicesSlotsBeginnings[id] = (u32*)pReserved;
    }
}


internal void
shrinkStackBlock(size_t id, MeshComponentType type, size_t oldSize, size_t newSize) {
    u8* pBuffer; 
    size_t* pOffset; 
    size_t bufferSize;
    size_t sizeOfType;
    u8* pElement;
    
    if (type == VERTICES) {
        pBuffer = (u8*)gpMeshComponentsPool->vertices;
        pOffset = &gpMeshComponentsPool->verticesOffset;
        bufferSize = MAX_VERTICES;
        sizeOfType = sizeof(Vertex);
        pElement = (u8*)gpMeshComponentsPool->verticesSlotsBeginnings[id];
    } else if (type == TEXTURES) {
        pBuffer = (u8*)gpMeshComponentsPool->pTextures;
        pOffset = &gpMeshComponentsPool->texturesOffset;
        bufferSize = MAX_TEXTURES_POINTERS;
        sizeOfType = sizeof(ModelTexture*);
        pElement = (u8*)gpMeshComponentsPool->texturesSlotsBeginnings[id];
    } else if (type == INDICES) {
        pBuffer = (u8*)gpMeshComponentsPool->indices;
        pOffset = &gpMeshComponentsPool->indicesOffset;
        bufferSize = MAX_INDICES;
        sizeOfType = sizeof(u32);
        pElement = (u8*)gpMeshComponentsPool->indicesSlotsBeginnings[id];
    } else {
        return;
    }
    
    if (newSize >= oldSize) {
        return;
    }
    
    if (pBuffer + ((*pOffset) * newSize)*sizeOfType < pBuffer + bufferSize*sizeOfType) {
        u8* pFrom = pElement + oldSize*sizeOfType;
        u8* pTo = pElement + newSize*sizeOfType;
        size_t chunkSize = *pOffset*sizeOfType - (pFrom - pBuffer);
        memmove(pTo, pFrom, chunkSize);
    } else {
        assert(false);
    }
    
    size_t idCount = 0;
    u32 size = sizeof(size_t);
    if (type == VERTICES) {
        if (newSize == 0) {
            gpMeshComponentsPool->verticesSlotsBeginnings[id] = 0;
        }
        for (size_t iID = id; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->verticesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount - id) {
                break;
            }
        }
    }
    else if (type == TEXTURES) {
        if (newSize == 0) {
            gpMeshComponentsPool->texturesSlotsBeginnings[id] = 0;
        }
        for (size_t iID = id; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->texturesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount - id) {
                break;
            }
        }
    }
    else if (type == INDICES) {
        if (newSize == 0) {
            gpMeshComponentsPool->indicesSlotsBeginnings[id] = 0;
        }
        for (size_t iID = id; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->indicesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount - id) {
                break;
            }
        }
    }
    
    if (gpMeshComponentsPool->verticesSlotsBeginnings[id] == 0 && 
        gpMeshComponentsPool->texturesSlotsBeginnings[id] == 0 && 
        gpMeshComponentsPool->indicesSlotsBeginnings[id] == 0) 
    {
        size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[id/size];
        size_t offset = id%size;
        *pCurrentWord = *pCurrentWord & ~(1 << offset);
        --gpMeshComponentsPool->idsCount;
    }
}

internal void
freeStackBlock(size_t id, MeshComponentType type, size_t oldSize) {
    shrinkStackBlock(id, type, oldSize, 0);
}

ENGINE_API void 
initMesh(Mesh* pMesh) {
    //initTransform(&pMesh->transform);
    
    initVA(&pMesh->va);
    vaBind(pMesh->va);
    
    size_t id = pMesh->meshComponentID;
    initVB(&pMesh->vb, gpMeshComponentsPool->verticesSlotsBeginnings[id], pMesh->verticesCount*sizeof(Vertex));
    vbBind(pMesh->vb);
    
    initEB(&pMesh->eb, gpMeshComponentsPool->indicesSlotsBeginnings[id], pMesh->indicesCount);
    ebBind(pMesh->eb);
    
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 3;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    
    vbLayoutPushFloat(&layout, 3);
    vbLayoutPushFloat(&layout, 3);
    vbLayoutPushFloat(&layout, 2);
    
    vaAddBuffer(pMesh->va, pMesh->vb, &layout);
    free(layout.pElements);
    
    vaUnbind();
}

ENGINE_API void
drawMesh(Mesh* pMesh) {
    u32 diffuseNr = 1;
    u32 specularNr = 1;
    u32 normalNr = 1;
    u32 reflectionNr = 1;
    u32 texturesSize = pMesh->texturesCount;
    meow_hash diffuseHash = MeowHash_Accelerated(0, sizeof(gpDiffuse), (void*)gpDiffuse);
    meow_hash specularHash = MeowHash_Accelerated(0, sizeof(gpSpecular), (void*)gpSpecular);
    meow_hash normalHash = MeowHash_Accelerated(0, sizeof(gpNormal),(void*)gpNormal);
    meow_hash reflectionHash = 
        MeowHash_Accelerated(0, sizeof(gpReflection),(void*)gpReflection);
    const char* pTypes[] = { gpDiffuse, gpSpecular, gpNormal, gpReflection }; 
    size_t id = pMesh->meshComponentID;
    
    char pName[512];
    for (u32 i = 0; i < texturesSize; ++i) {
        glCall(glActiveTexture(GL_TEXTURE0 + i));
        s32 number = 0;
        s32 typeIndex = 0;
        meow_hash type = pMesh->pModelTextures[i]->typeHash;
        
        if (MeowHashesAreEqual(type, diffuseHash)) {
            number = diffuseNr++;
            typeIndex = 0;
        } else if (MeowHashesAreEqual(type, specularHash)) {
            number = specularNr++;
            typeIndex = 1;
        } else if (MeowHashesAreEqual(type, normalHash)) {
            number = normalNr++;
            typeIndex = 2;
        } else if (MeowHashesAreEqual(type, reflectionHash)) {
            number = reflectionNr++;
            typeIndex = 3;
        }
        
        strcpy(pName, pTypes[typeIndex]);
        size_t iEndOfString = strlen(pTypes[typeIndex]);
        // NOTE(Marchin): I assume no model will have more than 99 textures of one kind
        char pNumber[2];
        _itoa(number, pNumber, 10);
        strcat(pName, pNumber);
        shaderSetInt(pMesh->pMaterial, pName, i);
        glCall(glBindTexture(GL_TEXTURE_2D, gpMeshComponentsPool->texturesSlotsBeginnings[id][i]->id));
    }
    vaBind(pMesh->va);
    drawElements(pMesh->indicesCount);
    vaUnbind();
}

ENGINE_API void
freeMesh(Mesh* pMesh) {
    freeEB(&pMesh->eb);
    freeVB(&pMesh->vb);
    freeVA(&pMesh->va);
    pMesh->pMaterial = 0;
    freeStackBlock(pMesh->meshComponentID,
                   VERTICES,
                   pMesh->verticesCount);
    freeStackBlock(pMesh->meshComponentID,
                   TEXTURES,
                   pMesh->texturesCount);
    freeStackBlock(pMesh->meshComponentID,
                   INDICES,
                   pMesh->indicesCount);
    pMesh->verticesCount = 0;
    pMesh->texturesCount = 0;
    pMesh->indicesCount = 0;
}

ENGINE_API void
drawModel(void* pModel, Renderer* pRenderer) {
    Model* pCastedModel = (Model*)pModel;
    if (pCastedModel->meshesCount <= 0) {
        return;
    }
    
    materialBindID(pCastedModel->pMaterial->id);
}

ENGINE_API u32 
textureFromFile(const char* pTextureName, const char* pModelPath) {
    Texture texture;
    char pTexturePath[MAX_PATH_SIZE];
    strcpy(pTexturePath, pModelPath);
    strcat(pTexturePath, pTextureName);
    initTexture(&texture, pTexturePath, true);
    
    return texture.id;
}

internal void
loadMaterialsTextures(Model* pModel, Mesh* pMesh, aiMaterial* pMaterial, 
                      aiTextureType type, const char* pTypeName) {
    
    u32 texturesCount = pMaterial->GetTextureCount(type);
    u32 modelTexturesCount = pModel->texturesCount;
    
    for (u32 i = 0; i < texturesCount; ++i) {
        b32 skip = false;
        aiString str;
        pMaterial->GetTexture(type, i, &str);
        for (u32 j = 0; j < modelTexturesCount; ++j) {
            if (strcmp(pModel->pLoadedTextures[j].pPath, str.C_Str()) == 0) {
                skip = true;
                
                *(pMesh->pModelTextures + pMesh->texturesCount + texturesCount - i - 1) = 
                    &pModel->pLoadedTextures[j];
                pMesh->texturesCount++;
                break;
            }
        }
        
        if (!skip) {
            ModelTexture* pModelTexture = &pModel->pLoadedTextures[pModel->texturesCount];
            pModelTexture->id = textureFromFile(str.C_Str(), pModel->pPath);
            pModelTexture->typeHash = 
                MeowHash_Accelerated(0, sizeof(pTypeName), (void*)pTypeName);
            strcpy(pModelTexture->pPath, str.C_Str());
            *(pMesh->pModelTextures + pMesh->texturesCount + texturesCount - i - 1) = 
                &pModel->pLoadedTextures[pModel->texturesCount++];
            pMesh->texturesCount++;
        }
    }
}

internal void 
setupModelVertex(aiMesh* pAiMesh, Mesh* pMesh) {
    Vertex vertex;
    Vertex* pVertices = pMesh->pVertices;
    u32 verticesCount = pAiMesh->mNumVertices;
    
    for (u32 iVertex = 0; iVertex < verticesCount; ++iVertex) {
        vertex.pos.x = pAiMesh->mVertices[iVertex].x;
        vertex.pos.y = pAiMesh->mVertices[iVertex].y;
        vertex.pos.z = pAiMesh->mVertices[iVertex].z;
        
        vertex.normal.x = pAiMesh->mNormals[iVertex].x;
        vertex.normal.y = pAiMesh->mNormals[iVertex].y;
        vertex.normal.z = pAiMesh->mNormals[iVertex].z;
        
        if (pAiMesh->mTextureCoords[0]) {
            vertex.uv.u = pAiMesh->mTextureCoords[0][iVertex].x;
            vertex.uv.v = pAiMesh->mTextureCoords[0][iVertex].y;
        } else {
            vertex.uv = hmm_vec2{0.0f, 0.0f};
        }
        
        pVertices[iVertex] = vertex;
    }
}

internal void
processMeshes(Model* pModel, const aiScene* pScene) {
    u32 meshesCount = pModel->meshesCount;
    for (u32 iMesh = 0; iMesh < meshesCount; ++iMesh) {
        Mesh* pMesh = &pModel->pMeshes[iMesh];
        aiMesh* pAiMesh = pScene->mMeshes[iMesh];
        u32 verticesCount = pAiMesh->mNumVertices;
        u32 facesCount = pAiMesh->mNumFaces;
        u32 indicesCount = 0;
        
        *pMesh = {};
        
        size_t id = getMeshComponentID();
        pMesh->meshComponentID = id; 
        
        
        pushToStack(pMesh->meshComponentID, 
                    VERTICES, 
                    verticesCount);
        
        pMesh->pVertices = gpMeshComponentsPool->verticesSlotsBeginnings[id];
        
        
        for (u32 iFace = 0; iFace < facesCount; ++iFace) {
            indicesCount += pAiMesh->mFaces[iFace].mNumIndices;
        }
        pushToStack(pMesh->meshComponentID, 
                    INDICES, 
                    indicesCount);
        
        pMesh->pIndices = gpMeshComponentsPool->indicesSlotsBeginnings[id];
        
        setupModelVertex(pAiMesh, pMesh);
        
        pMesh->verticesCount = verticesCount;
        
        
        u32 iIndex = 0;
        for (u32 i = 0; i < facesCount; ++i) {
            aiFace face = pAiMesh->mFaces[i];
            u32 faceIndicesCount = face.mNumIndices;
            
            if (faceIndicesCount != 3) {
                continue;
            }
            
            memcpy(&pMesh->pIndices[iIndex], face.mIndices, faceIndicesCount*sizeof(u32));
            iIndex += faceIndicesCount;
        }
        
        pMesh->indicesCount = indicesCount;
        
        u32 materialIndex = pAiMesh->mMaterialIndex;
        aiMaterial* pMaterial = pScene->mMaterials[materialIndex];
        
        u32 texturesCount = 0;
        pMesh->texturesCount = 0;
        if (materialIndex >= 0) {
            texturesCount = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
            pushToStack(pMesh->meshComponentID,
                        TEXTURES, 
                        texturesCount);
            pMesh->pModelTextures = gpMeshComponentsPool->texturesSlotsBeginnings[id];
            loadMaterialsTextures(pModel, pMesh, pMaterial, 
                                  aiTextureType_DIFFUSE, gpDiffuse);
            
#if 0            
            texturesCount = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
            pushToStack(pMesh->meshComponentID,
                        TEXTURES, 
                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_SPECULAR, gpSpecular);
            
            texturesCount = pMaterial->GetTextureCount(aiTextureType_NORMALS);
            pushToStack(pMesh->meshComponentID,
                        TEXTURES, 
                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_NORMALS, gpNormal);
            
            texturesCount = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
            pushToStack(pMesh->meshComponentID,
                        TEXTURES, 
                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_AMBIENT, gpReflection);
#endif
        }
        
        pMesh->pMaterial = pModel->pMaterial;
        
        initMesh(pMesh);
    }
}

ENGINE_API b32
IsMeshInsideFrustum(Mesh* pMesh, Camera* pCamera, Renderer* pRenderer) {
    u32 verticesCount = pMesh->verticesCount;
    
    for (u32 iVertex = 0; iVertex < verticesCount; ++iVertex) {
        hmm_vec4 wcVertex4 = 
            HMM_MultiplyMat4ByVec4(getModelView(pRenderer),
                                   HMM_Vec4v(pMesh->pVertices[iVertex].pos, 1.f));
        hmm_vec3 wcVertex3 = {wcVertex4.X, wcVertex4.Y, wcVertex4.Z};
        if (IsPointInsideFrustum(wcVertex3, pCamera)) {
            return true;
        }
    }
    
    return false;
}

ENGINE_API void
drawModelNode(void* pNode, Renderer* pRenderer) {
    ModelNode* pCastedNode = (ModelNode*)pNode;
    
    u32 meshCount = pCastedNode->meshIndicesCount;
    Mesh* pMeshes = pCastedNode->pModel->pMeshes;
    
    pRenderer->pCamera->model = pCastedNode->transform.model;
    
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(pCastedNode->pMaterial, 
                  "uModelViewProjection", 
                  &mvp);
    
    b32 drawable = false;
    for (u32 iMesh = 0; iMesh < meshCount; ++iMesh) {
        drawable = IsMeshInsideFrustum(&pMeshes[pCastedNode->pMeshIndices[iMesh]], 
                                       pRenderer->pCamera, pRenderer);
        
        if (drawable) { 
            drawMesh(&pMeshes[pCastedNode->pMeshIndices[iMesh]]);
            ++gDrawed;
        }
    }
    if (meshCount > 0) {
        printf("%d\n", gDrawed);
        gDrawed = 0;
    }
}

internal void
processNode(Model* pModel, aiNode* pNode, Transform* pParent) {
    u32 childCount = pNode->mNumChildren;
    ModelNode* pModelNode = &pModel->pNodes[pModel->nodesCount++];
    initTransform(&pModelNode->transform);
    addChild(&pModelNode->transform, pParent);
    
    pModelNode->meshIndicesCount = pNode->mNumMeshes;
    u32 sizeOfIndicesInBytes = pModelNode->meshIndicesCount*sizeof(u32);
    pModelNode->pMeshIndices = (u32*)malloc(sizeOfIndicesInBytes);
    memcpy(pModelNode->pMeshIndices, pNode->mMeshes, sizeOfIndicesInBytes);
    pModelNode->pModel = pModel;
    pModelNode->pMaterial = pModel->pMaterial;
    pModelNode->transform.pEntity = pModelNode;
    pModelNode->transform.draw = drawModelNode;
    
    for (u32 iChild = 0; iChild < childCount; ++iChild) {
        processNode(pModel, pNode->mChildren[iChild], &pModelNode->transform);
    }
}

internal u32
countNodes(aiNode* pNode) {
    u32 childCount = pNode->mNumChildren;
    u32 amount = 1;
    for (u32 iChild = 0; iChild < childCount; ++iChild) {
        amount += countNodes(pNode->mChildren[iChild]);
    }
    
    return amount;
}

internal void
freeNode(ModelNode* pNode) {
    u32 childCount = pNode->transform.childrenCount;
    free(pNode->pMeshIndices);
    for (u32 iChild = 0; iChild < childCount; ++iChild) {
        freeNode((ModelNode*)(pNode->transform.pChildren[iChild]->pEntity));
    }
}

ENGINE_API void 
initModel(Model* pModel, const char* pPath, 
          Material* pMaterial, Transform* pTransform) {
    Assimp::Importer importer;
    const aiScene* pScene = 
        importer.ReadFile(pPath, aiProcess_Triangulate);// | aiProcess_FlipUVs);
    
    if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
        printf(importer.GetErrorString());
    }
    
    size_t index;
    size_t size;
    if (findCharacter(pPath, '/', LAST, &index)) {
        size = sizeof(char)*(index + 1);
    } else {
        size = sizeof(pPath);
    }
    memcpy(pModel->pPath, pPath, size);
    pModel->component.id = MODEL;
    pModel->component.draw = drawModel;
    pModel->pPath[size] = '\0';
    pModel->meshesCount = pScene->mNumMeshes;
    pModel->texturesCount = pScene->mNumTextures;
    pModel->pMeshes = (Mesh*)malloc(pModel->meshesCount*sizeof(Mesh));
    pModel->pLoadedTextures = 
        (ModelTexture*)malloc(48*sizeof(ModelTexture));
    pModel->pTransform = pTransform;
    pModel->pMaterial = pMaterial;
    
    processMeshes(pModel, pScene);
    
    int nodesCount = countNodes(pScene->mRootNode);
    pModel->pNodes = (ModelNode*)malloc(nodesCount*sizeof(ModelNode));
    memset(pModel->pNodes, 0, nodesCount*sizeof(ModelNode));
    
    pModel->nodesCount = 0;
    processNode(pModel, pScene->mRootNode, pModel->pTransform);
    
    pModel->pLoadedTextures = 
        (ModelTexture*)realloc(pModel->pLoadedTextures, 
                               pModel->texturesCount*sizeof(ModelTexture));
}

ENGINE_API void
freeModel(Model* pModel) {
    u32 meshesCount = pModel->meshesCount;
    for (u32 iMesh = 0; iMesh < meshesCount; ++iMesh) {
        freeMesh(&pModel->pMeshes[iMesh]);
    }
    pModel->meshesCount = 0;
    
    u32 texturesCount = pModel->texturesCount;
    for (u32 iTexture = 0; iTexture < texturesCount; ++iTexture){
        ModelTexture* pTexture = &pModel->pLoadedTextures[iTexture];
        freeTexture(&pTexture->id);
        *pTexture = {};
    }
    pModel->texturesCount = 0;
    
    pModel->pPath[0] = '\0';
    
    freeNode(&pModel->pNodes[0]);
    
    free(pModel->pNodes);
}