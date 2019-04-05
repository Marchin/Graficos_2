ENGINE_API inline b32
checkTilemapFormat(const char* pFileDir) {
    //check if it is .csv
    //get to the end of the direction
    i32 iter = 0;
    while (*(pFileDir + iter) != '\0') {
        iter++;
    }
    
    if (*(pFileDir + --iter) != 'v') {
        return false;
    } else if (*(pFileDir + --iter) != 's') {
        return false;
    } else if (*(pFileDir + --iter) != 'c') {
        return false;
    } else if (*(pFileDir + --iter) != '.') {
        return false;
    } else {
        return true;
    }
}

ENGINE_API b32
loadTilemap(Tilemap* pTilemap, const char* pFileDir) {
    if (!checkTilemapFormat(pFileDir)) {
        printf("Error Loading Tilemap: Invalid format\n");
        return false;
    } 
    
    FILE* pFile = fopen(pFileDir, "r");
    char* pTileInfo;
    u32 fileSize;
    
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        fileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        pTileInfo  = (char*)malloc(fileSize);
        if (pTileInfo && fileSize) {
            fread(pTileInfo, sizeof(char), fileSize, pFile);
        }
        while (pTileInfo[fileSize - 1] < '\0' && fileSize > 0) {
            --fileSize; // NOTE(Marchin): for some reason some text files have 
            //non-valid characters at EOF, so we get rid of them
        }
        fclose(pFile);
    } else {
        perror("Error opening file\n");
        return false;
    }
    
    pTilemap->width = 0;
    pTilemap->height = 0;
    
    b32 widthIsCalulated = false;
    for (u32 iChar = 0; iChar < fileSize; ++iChar) {
        if (*(pTileInfo + iChar) == '\n') {
            if (!widthIsCalulated) {
                ++pTilemap->width;
                widthIsCalulated = true;
            }
            ++pTilemap->height;
        } 
        if (!widthIsCalulated) {
            if (*(pTileInfo + iChar) == ',') {
                ++pTilemap->width;
            }
        }
    }
    pTilemap->pIDs = (i32*)malloc(pTilemap->width*pTilemap->height*sizeof(i32));
    i32 id = 0;
    for (u32 iChar = 0; iChar < fileSize; ++iChar) {
        b32 idFound = false;
        b32 isNegative = false;
        i32 newID = 0; //accumulator
        i32 decimal = 1; //multiplies the value of the character 
        
        if (*(pTileInfo + iChar) == '-') {
            isNegative = true;
            ++iChar;
        }
        u32 iter = iChar;
        while ((iter < fileSize) && (*(pTileInfo + iter) != ',') && 
               (*(pTileInfo + iter) != '\n')) {
            
            ++iter;
        }
        u32 diff = iter - iChar;
        for (u32 j = 0; j < diff; ++j) {
            //to take into account it's position (1, 10, 100, etc)
            newID += (*(pTileInfo + iter - j - 1) - '0') * decimal; 
            //read number from right to left
            // -'0' to convert it from ASCII to int
            decimal *= 10;
        }
        
        if (isNegative) {
            newID = -newID;
        }
        pTilemap->pIDs[id] = newID;
        ++id;
        iChar = iter;
    }
    free(pTileInfo);
    
    return true;
}

ENGINE_API inline void
tilemapWCoordsToColRow(Tilemap* pTilemap, f32 wcX, f32 wcY, i32* pCol, i32* pRow) {
    *pCol = (i32)((wcX ) / pTilemap->tileSize);
    *pRow = (i32)(-(wcY ) / pTilemap->tileSize);
}

ENGINE_API inline void
tilemapColRowToWCoords(Tilemap* pTilemap, u32 col, u32 row, f32* pWCX, f32* pWCY) {
    *pWCX = (f32)col*pTilemap->tileSize;
    *pWCY = -(f32)row*pTilemap->tileSize;
}

ENGINE_API void
tilemapCheckSideColliding(Tilemap* pTilemap, BoxCollider* pCollider, 
                          f32 offsetX, f32 offsetY) {
    i32 col, row;
    f32 tileX, tileY;
    f32 deltaX = 0.f;
    f32 deltaY = 0.f;
    tilemapWCoordsToColRow(pTilemap, 
                           pCollider->pTransform->position.x + offsetX,
                           pCollider->pTransform->position.y + offsetY, 
                           &col, &row);
    tilemapColRowToWCoords(pTilemap, col, row, &tileX, &tileY);
    i32 tileID = pTilemap->pIDs[col + pTilemap->width* row];
    if (col < 0 || row < 0 || col >= pTilemap->width || row >= pTilemap->height) {
        return;
    }       
    size_t bitmaskSize = sizeof(size_t);
    if ((pTilemap->collisionBitmask[tileID/bitmaskSize] >> 
         (tileID%bitmaskSize))&(size_t)1) {
        
        if (offsetX != 0.f) {
            deltaX = pCollider->pTransform->position.x + offsetX - tileX;
            if (signbit(deltaX) != signbit(offsetX)) { //tells from where i'm measuring
                deltaX = 1.f - HMM_ABS(deltaX);
            } else {
                deltaX = HMM_ABS(deltaX);
            }
            deltaX = (signbit(offsetX) ? deltaX : -deltaX);
        }
        if (offsetY != 0.f) {
            deltaY = pCollider->pTransform->position.y + offsetY - tileY;
            if (signbit(deltaY) != signbit(offsetY)) { 
                deltaY = 1.f - HMM_ABS(deltaY);
            } else {
                deltaY = HMM_ABS(deltaY);
            }
            deltaY = (signbit(offsetY) ? deltaY : -deltaY);
        }
    }
    if (deltaX != 0.f || deltaY != 0.f) {
        transformTranslate(pCollider->pTransform, deltaX, deltaY, 0.f);
    }
}

ENGINE_API void
tilemapCheckCollisions(Tilemap* pTilemap) {
    for (u32 i = 0; i < pTilemap->collidersCount;) {
        if (pTilemap->colliders[i] != 0) {
            tilemapCheckSideColliding(pTilemap,
                                      pTilemap->colliders[i], 
                                      pTilemap->colliders[i]->halfWidth, 
                                      0.f);
            tilemapCheckSideColliding(pTilemap,
                                      pTilemap->colliders[i],
                                      -pTilemap->colliders[i]->halfWidth, 
                                      0.f);
            tilemapCheckSideColliding(pTilemap,
                                      pTilemap->colliders[i], 
                                      0.f, 
                                      pTilemap->colliders[i]->halfHeight);
            tilemapCheckSideColliding(pTilemap,
                                      pTilemap->colliders[i], 
                                      0.f, 
                                      -pTilemap->colliders[i]->halfHeight);
            ++i;
        }
    }
}

ENGINE_API inline void
tilemapSetCollisionableTiles(Tilemap* pTilemap, i32* pFrames, i32 count) {
    size_t bitmaskSize = sizeof(size_t);
    for (size_t iFrame = 0; iFrame < count; ++iFrame) {
        size_t frame = *(pFrames + iFrame);
        pTilemap->collisionBitmask[frame/bitmaskSize] |= ((size_t)1 << (frame%bitmaskSize));
    }
}

ENGINE_API inline b32
tilemapRegisterColliders(Tilemap* pTilemap, BoxCollider* pCollider) {
    for (i32 i = 0; i < MAX_COLLIDERS; ++i) {
        if (pTilemap->colliders[i] == 0) {
            pTilemap->colliders[i] = pCollider;
            ++pTilemap->collidersCount;
            return true;
        }
    }
    return false;
}

ENGINE_API void
tilemapCalculateVertexPosition(Tilemap* pTilemap) {
    pTilemap->pCoords = (f32*)malloc(sizeof(f32)*pTilemap->width*pTilemap->height * 4 * 3);
    f32 posX = 0.f;
    f32 posY = 0.f;
    i32 count = 0;
    for (i32 i = 0; i < pTilemap->height; ++i) {
        for (i32 j = 0; j < pTilemap->width; ++j) {
            pTilemap->pCoords[count*4*3] = posX;
            pTilemap->pCoords[count*4*3 + 1] = posY - pTilemap->tileSize;
            pTilemap->pCoords[count*4*3 + 2] = 0.f;
            pTilemap->pCoords[count*4*3 + 3] = posX;
            pTilemap->pCoords[count*4*3 + 4] = posY;
            pTilemap->pCoords[count*4*3 + 5] = 0.f;
            pTilemap->pCoords[count*4*3 + 6] = posX + pTilemap->tileSize;
            pTilemap->pCoords[count*4*3 + 7] = posY - pTilemap->tileSize;
            pTilemap->pCoords[count*4*3 + 8] = 0.f;
            pTilemap->pCoords[count*4*3 + 9] = posX + pTilemap->tileSize;
            pTilemap->pCoords[count*4*3 + 10] = posY;
            pTilemap->pCoords[count*4*3 + 11] = 0.f;
            ++count;
            posX += pTilemap->tileSize;
        }
        posX = 0.f;
        posY -= pTilemap->tileSize;
    }
    vbSetData(pTilemap->vbPosition, pTilemap->pCoords, 
              sizeof(f32) * pTilemap->width * pTilemap->height * 4 * 3);
}

ENGINE_API void
tilemapCalculateUV(Tilemap* pTilemap) {
    u32 id = 0;
    pTilemap->pUVData = (f32*)malloc(sizeof(f32) * pTilemap->width * pTilemap->height * 8);
    for (i32 i = 0; i < pTilemap->width * pTilemap->height; ++i) {
        if (pTilemap->pIDs[i] == -1) {
            for (i32 j = 0; j < 8; ++j) {
                pTilemap->pUVData[i * 8 + j] = -1;
            }
        } else {
            Coords uvCoords = pTilemap->tileset.pUVCoords[pTilemap->pIDs[i]];
            pTilemap->pUVData[i*8] = uvCoords.u0;
            pTilemap->pUVData[i*8 + 1] = uvCoords.v1;
            pTilemap->pUVData[i*8 + 2] = uvCoords.u0;
            pTilemap->pUVData[i*8 + 3] = uvCoords.v0;
            pTilemap->pUVData[i*8 + 4] = uvCoords.u1;
            pTilemap->pUVData[i*8 + 5] = uvCoords.v1;
            pTilemap->pUVData[i*8 + 6] = uvCoords.u1;
            pTilemap->pUVData[i*8 + 7] = uvCoords.v0;
        }
    }
    vbSetData(pTilemap->vbUV, pTilemap->pUVData, 
              sizeof(f32) * pTilemap->width * pTilemap->height * 8);
}

ENGINE_API void
tilemapCalculateVisibleTiles(Tilemap* pTilemap, Renderer* pRenderer) {
    i32 count = 0;
    i32 tilesUntilCamX, tilesUntilCamY;
    tilemapWCoordsToColRow(pTilemap, 
                           pTilemap->cacheCamPosX - getCameraWidth(pRenderer) * 0.5f, 
                           pTilemap->cacheCamPosY + getCameraHeight(pRenderer) * 0.5f, 
                           &tilesUntilCamX, &tilesUntilCamY);
    for (i32 i = 0; i < pTilemap->visibleHeight; ++i) {
        for (i32 j = 0; j < pTilemap->visibleWidth; ++j) {
            if ((tilesUntilCamX + j < 0) || (tilesUntilCamY + i < 0) ) {
                pTilemap->pVisibleTiles[count] = -1;
            } else {
                pTilemap->pVisibleTiles[count] = 
                    tilesUntilCamX + j + (pTilemap->width*(i + tilesUntilCamY));
            }
            ++count;
        }
    }
}

ENGINE_API void
initTilemap(Tilemap* pTilemap, const char* pTilemapDir, SpriteSheet* pTileset, 
            Shader* pMaterial, Renderer* pRenderer) {
    *pTilemap = {};
    pTilemap->tileSize = 1.f;
    
    initTransform(&pTilemap->transform);
    initVA(&pTilemap->va);
    vaBind(pTilemap->va);
    initVB(&pTilemap->vbPosition);
    initVB(&pTilemap->vbUV);
    
    pTilemap->material = *pMaterial;
    pTilemap->tileset = *pTileset;
    if (!loadTilemap(pTilemap, pTilemapDir)) {
        Assert(0);
    }
    pTilemap->visibleWidth = (i32)(getCameraWidth(pRenderer) / pTilemap->tileSize) + 2; //one extra column at each side
    pTilemap->visibleHeight = (i32)(getCameraHeight(pRenderer) / pTilemap->tileSize) + 2; //one extra row at each side
    pTilemap->pVisibleTiles = 
        (i32*)malloc(sizeof(i32)*pTilemap->visibleWidth*pTilemap->visibleHeight);
    hmm_vec3 cameraPos = getCameraPosition(pRenderer);
    pTilemap->cacheCamPosX= cameraPos.x;
    pTilemap->cacheCamPosY = cameraPos.y;
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
    
    tilemapCalculateVertexPosition(pTilemap);
    tilemapCalculateUV(pTilemap);
    tilemapCalculateVisibleTiles(pTilemap, pRenderer);
    
    vaAddBufferByLocation(pTilemap->va, pTilemap->vbPosition, &layout, 0);
    vaAddBufferByLocation(pTilemap->va, pTilemap->vbUV, &layout2, 1);
    free(layout.pElements);
    free(layout2.pElements);
}

ENGINE_API inline void
freeTilemap(Tilemap* pTilemap) {
    if (pTilemap->pIDs != 0) {
        free(pTilemap->pIDs);
    }
    if (pTilemap->pUVData != 0) {
        free(pTilemap->pUVData);
    }
    if (pTilemap->pVisibleTiles != 0) {
        free(pTilemap->pVisibleTiles);
    }
    if (pTilemap->pCoords != 0) {
        free(pTilemap->pCoords);
    }
}

ENGINE_API void
drawTilemap(Tilemap* pTilemap, Renderer* pRenderer) {
    hmm_vec3 pos = getCameraPosition(pRenderer);
    f32 camPosX = pos.x;
    f32 camPosY = pos.y;
    
    if ((HMM_ABS((pTilemap->cacheCamPosX - camPosX)) >= 1.f) ||
        (HMM_ABS((pTilemap->cacheCamPosY - camPosY)) >= 1.f)) {
        
        pTilemap->cacheCamPosX = camPosX;
        pTilemap->cacheCamPosY = camPosY;
        tilemapCalculateVisibleTiles(pTilemap, pRenderer);
    }
    shaderBindID(pTilemap->material.id);
    textureBindID(pTilemap->tileset.spriteRenderer.texture.id, 0);
    pRenderer->model = pTilemap->transform.model;
    hmm_mat4 mvp = getModelViewProj(pRenderer);
    shaderSetMat4(&pTilemap->material, "uModelViewProjection", &mvp);
    vaBind(pTilemap->va);
    for (i32 i = 0; i < pTilemap->visibleHeight; ++i) {
        for (i32 j = 0; j < pTilemap->visibleWidth; ++j) {
            if (pTilemap->pVisibleTiles[(j + (pTilemap->visibleWidth*i))] >= 0) {
                drawBufferStrip(
                    pTilemap->pVisibleTiles[(j + (pTilemap->visibleWidth*i))] * 4, 4);
            }
        }
    }
}
