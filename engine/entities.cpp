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
initTriangle(Triangle* pTriangle, Shader* pMaterial, const void* pData, u32 size) {
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
    shaderBindID(pTriangle->material.id);
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
initColorSquare(ColorSquare* pCS, Shader* pMaterial, 
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
	shaderBindID(pCS->material.id); 
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
initCircle(Circle* pCircle, Shader* pMaterial, u32 sidesAmount, f32 radius) {
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
	shaderBindID(pCircle->material.id);
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
initSpriteRenderer(SpriteRenderer* pSR, Shader* pMaterial, 
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
    shaderBindID(pSR->material.id);
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
	shaderBindID(pSR->material.id);
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
initSpriteSheet(SpriteSheet* pSS, Shader* pMaterial, const char* pTexturePath, 
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
		Assert(pSS->frameWidth != 0);
		pSS->columns = pSS->spriteRenderer.texture.width / pSS->frameWidth;
	}
	if (pSS->rows == 0) {
		Assert(pSS->frameHeight != 0);
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