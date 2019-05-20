ENGINE_API inline void 
transformUpdateMC(Transform* pTransform) {
	pTransform->model = 
        pTransform->positionMatrix * pTransform->rotationMatrix * pTransform->scaleMatrix;
}

ENGINE_API void
initTransform(Transform* pTransform) {
    pTransform->positionMatrix = HMM_Mat4d(1.f);
    pTransform->rotationMatrix = HMM_Mat4d(1.f);
    pTransform->scaleMatrix = HMM_Mat4d(1.f);
    pTransform->position = HMM_Vec3T(0.f);
    pTransform->eulerAngles = HMM_Vec3T(0.f);
    pTransform->scale = HMM_Vec3T(1.f);
    
	transformUpdateMC(pTransform);
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


////////////////////////////////

//TRIANGLE

////////////////////////////////

ENGINE_API void
initTriangle(Triangle* pTriangle, Material* pMaterial, const void* pData, u32 size) {
    initTransform(&pTriangle->transform);
    initVA(&pTriangle->va);
    vaBind(pTriangle->va);
    initVB(&pTriangle->vb, pData, size);
    pTriangle->material = *pMaterial;
	
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
drawTriangle(Triangle* pTriangle, Renderer* pRenderer) {
    materialBindID(pTriangle->material.id);
	pRenderer->pCamera->model = pTriangle->transform.model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(&pTriangle->material, 
                  "uModelViewProjection", 
                  &mvp);
	vaBind(pTriangle->va);
	drawBufferStrip(0, 3);
}

ENGINE_API inline void 
setTriangleVertices(Triangle* pTriangle, const void* pData) {
	vbSetData(pTriangle->vb, pData, 12 * sizeof(f32));
}


////////////////////////////////

//COLOR SQUARE

////////////////////////////////

ENGINE_API void
initColorSquare(ColorSquare* pCS, Material* pMaterial, 
                const void* pPosition, const void* pColor) {
    
    initTransform(&pCS->transform);
    initVA(&pCS->va);
    vaBind(pCS->va);
    initVB(&pCS->vbPosition, pPosition, 12 * sizeof(f32));
    initVB(&pCS->vbColor, pColor, 12 * sizeof(f32));
    pCS->material = *pMaterial;
	
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
drawColorSquare(ColorSquare* pCS, Renderer* pRenderer) {
	materialBindID(pCS->material.id); 
	pRenderer->pCamera->model = pCS->transform.model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
	shaderSetMat4(&pCS->material, "uModelViewProjection", &mvp);
	vaBind(pCS->va);
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
initCircle(Circle* pCircle, Material* pMaterial, u32 sidesAmount, f32 radius) {
    pCircle->sides = sidesAmount;
    pCircle->radius = radius;
    pCircle->material = *pMaterial;
    
    initTransform(&pCircle->transform);
    initVA(&pCircle->va);
    vaBind(pCircle->va);
    initVB(&pCircle->vb);
    pCircle->material = *pMaterial;
	
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
drawCircle(Circle* pCircle, Renderer* pRenderer) {
	materialBindID(pCircle->material.id);
	pRenderer->pCamera->model = pCircle->transform.model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
	shaderSetMat4(&pCircle->material, "uModelViewProjection", &mvp);
	vaBind(pCircle->va);
	drawBufferFan(0, pCircle->sides + 2);
}

////////////////////////////////

//SPRITERENDERER

////////////////////////////////

ENGINE_API void
initSpriteRenderer(SpriteRenderer* pSR, Material* pMaterial, 
                   const char* pTexturePath,
                   const void* pPosition, const void* pUV) {
    
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
    pSR->material = *pMaterial;
    materialBindID(pSR->material.id);
    textureBindID(pSR->texture.id, 0);
    shaderSetInt(&pSR->material, "tex", 0);
    
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
drawSpriteRenderer(SpriteRenderer* pSR, const Transform* pTransform, Renderer* pRenderer) {
	materialBindID(pSR->material.id);
	textureBindID(pSR->texture.id, 0);
	pRenderer->pCamera->model = pTransform->model;
	hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(&pSR->material, "uModelViewProjection", &mvp);
	vaBind(pSR->va);
	drawBufferStrip(0, 4);
}


////////////////////////////////

//SPRITESHEET

////////////////////////////////

ENGINE_API void
initSpriteSheet(SpriteSheet* pSS, Material* pMaterial, const char* pTexturePath, 
                const void* pPosition, const void* pUV) { 
    
    *pSS = {};
    initSpriteRenderer(&pSS->spriteRenderer, pMaterial, pTexturePath, pPosition, pUV);
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

ENGINE_API void
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
updateAnimation(Animation* pAnimation, f32 deltaTime) {
	pAnimation->counter += deltaTime;
	if (pAnimation->counter >= pAnimation->interval) {
		pAnimation->currentFrame++;
		pAnimation->currentFrame %= pAnimation->count;
		spriteSheetSetFrame(pAnimation->pSS,
                            *(pAnimation->pFrames + pAnimation->currentFrame));
		pAnimation->counter = 0.f;
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
        for (size_t iID = id + 1; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->verticesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount) {
                break;
            }
        }
    }
    else if (type == TEXTURES) {
        if (newSize == 0) {
            gpMeshComponentsPool->texturesSlotsBeginnings[id] = 0;
        }
        for (size_t iID = id + 1; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->texturesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount) {
                break;
            }
        }
    }
    else if (type == INDICES) {
        if (newSize == 0) {
            gpMeshComponentsPool->indicesSlotsBeginnings[id] = 0;
        }
        for (size_t iID = id + 1; iID < MAX_IDS; ++iID) {
            size_t* pCurrentWord = &gpMeshComponentsPool->idsUsed[iID/size];
            size_t offset = iID%size;
            if (((((*pCurrentWord  >> offset)) & 1) == 1)) {
                gpMeshComponentsPool->indicesSlotsBeginnings[iID] -= (oldSize - newSize);
                ++idCount;
            }
            
            if (idCount >= gpMeshComponentsPool->idsCount) {
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








inline b32
operator==(Vertex vA, Vertex vB) {
    b32 result = ((vA.pos == vB.pos) && (vA.uv == vB.uv));
    
    return result;
}

ENGINE_API void 
initMesh(Mesh* pMesh) {
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
        // NOTE(Marchin): I assume no model will have more that 99 textures of one kind
        char pNumber[2];
        _itoa(number, pNumber, 10);
        strcat(pName, pNumber);
        //materialBindID(pMesh->pMaterial->id);
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
drawModel(Model* pModel, Renderer* pRenderer) {
    if (pModel->meshesCount <= 0) {
        return;
    }
    
    materialBindID(pModel->material.id);
    Mesh* pMeshes = pModel->pMeshes;
	pRenderer->pCamera->model = pModel->transform.model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(&pModel->material, 
                  "uModelViewProjection", 
                  &mvp);
    u32 meshesCount = pModel->meshesCount;
    for (u32 iMesh = 0; iMesh < meshesCount; ++iMesh) {
        drawMesh(&pMeshes[iMesh]);
    }
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

ENGINE_API void 
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
#if 0
        b32 isNew = true;
        u32 uniqueVerticesCount = pMesh->verticesCount;
        u32* pIndices = pMesh->pIndices;
        for (u32 iUniqueVertex = 0; iUniqueVertex < uniqueVerticesCount; ++iUniqueVertex) {
            
            if (pVertices[iUniqueVertex] == vertex) {
                isNew = false;
                pIndices[pMesh->indicesCount++] = iUniqueVertex;
                // TODO(Marchin): add indices
                break;
            }
        }
        if (isNew) {
            pIndices[pMesh->indicesCount++] = uniqueVerticesCount; 
            pVertices[pMesh->verticesCount++] = vertex;
        }
#endif
    }
}

ENGINE_API void
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
#if 0        
        shrinkStackBlock(gpMeshComponentsPool->indices, 
                         &gpMeshComponentsPool->indicesOffset, 
                         MAX_INDICES, 
                         sizeof(u32),
                         pMesh->pIndices,
                         indicesCount,
                         pMesh->indicesCount);
#endif
        
#if 1        
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
#endif
        
        u32 materialIndex = pAiMesh->mMaterialIndex;
        aiMaterial* pMaterial = pScene->mMaterials[materialIndex];
        
        u32 texturesCount = 0;
        pMesh->texturesCount = 0;
        //ModelTexture*** pMeshTextures = &pMesh->pModelTextures;
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
            pushToStack(gpMeshComponentsPool->pTextures, 
                        &gpMeshComponentsPool->texturesOffet, 
                        MAX_TEXTURES_POINTERS, 
                        sizeof(ModelTexture*), 
                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_SPECULAR, gpSpecular);
            
            texturesCount = pMaterial->GetTextureCount(aiTextureType_NORMALS);
            pushToStack(gpMeshComponentsPool->pTextures, 
                        &gpMeshComponentsPool->texturesOffet, 
                        MAX_TEXTURES_POINTERS, 
                        sizeof(ModelTexture*), 
                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_NORMALS, gpNormal);
            
            texturesCount = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
            (ModelTexture**)pushToStack(gpMeshComponentsPool->pTextures, 
                                        &gpMeshComponentsPool->texturesOffet, 
                                        MAX_TEXTURES_POINTERS, 
                                        sizeof(ModelTexture*), 
                                        texturesCount);
            loadMaterialsTextures(pModel, pMesh, pMaterial,
                                  aiTextureType_AMBIENT, gpReflection);
#endif
        }
        
        pMesh->pMaterial = &pModel->material;
        
        initMesh(pMesh);
    }
}
/*
internal void
processNode(Model* pModel, aiNode* pNode, const aiScene* pScene) {
    u32 meshesCount = pNode->mNumMeshes;
    for (u32 i = 0; i < meshesCount; ++i) {
        pModel->pMeshes[i] = processMesh(pModel, pNode->mMeshes[i], pScene);
    }
    u32 nodeChildCount = pNode->mNumChildren;
    for (u32 i = 0; i < nodeChildCount; ++i) {
        processNode(pModel, pNode->mChildren[i], pScene);
    }
}
*/
ENGINE_API void 
initModel(Model* pModel, const char* pPath, const Material* pMaterial) {
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
    pModel->pPath[size] = '\0';
    pModel->meshesCount = pScene->mNumMeshes;
    pModel->texturesCount = pScene->mNumTextures;
    pModel->pMeshes = (Mesh*)malloc(pModel->meshesCount*sizeof(Mesh));
    pModel->pLoadedTextures = 
        (ModelTexture*)malloc(48*sizeof(ModelTexture));
    pModel->material = *pMaterial;
    //processNode(pModel, pScene->mRootNode, pScene);
    
    initTransform(&pModel->transform);
    
    processMeshes(pModel, pScene);
    
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
    pModel->material = {};
}