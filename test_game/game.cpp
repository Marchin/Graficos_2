internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->camX = {};
    pGame->camY = {};
    
    Material basicMaterial;
    initMaterial(&basicMaterial, "Shader" ,"..//resources//shaders//vShader.glsl", 
                 "..//resources//shaders//fShader.glsl");
    
    Material colorMaterial;
    initMaterial(&colorMaterial, "Color", "..//resources//shaders//vColor.glsl", 
                 "..//resources//shaders//fColor.glsl");
    
    Material textureMaterial;
    initMaterial(&textureMaterial, "Texture", "..//resources//shaders//vTexture.glsl", 
                 "..//resources//shaders//fTexture.glsl");
    
    Material modelMaterial;
    initMaterial(&modelMaterial, "Model", "..//resources//shaders//vModel.glsl", 
                 "..//resources//shaders//fModel.glsl");
    
    const char* pSpritePath = "..//resources//container.jpg";
    const char* pSpriteSheetPath = "..//resources//spriteSheet.png";
    
    float vertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.f, 0.f
	};
    float squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f,  0.5f, 0.f
	};
	float squareColors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 1.f, 0.f
	};
	float squareUV[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f
	};
    u32 frames[] = {
        0, 7, 2, 5, 4,
    };
    
    initModel(&pGame->model, "../resources/mesa_rol/MesaRol.obj", &modelMaterial);
    initModel(&pGame->model2, "../resources/nanosuit_reflection/nanosuit.obj", &modelMaterial);
    //initModel(&pGame->model2, "../resources/cube.obj", &modelMaterial);
    //initModel(&pGame->model2, "../resources/bath.obj", &modelMaterial);
    freeModel(&pGame->model);
    
#if 0
    initTriangle(&pGame->triangle, &basicMaterial, &vertices, sizeof(vertices));
    transformScale(&pGame->triangle.transform, 9.f, 3.f, 3.f);
    initColorSquare(&pGame->cs, &colorMaterial, &squareVertices, squareColors);
    transformScale(&pGame->cs.transform, 5.f, 5.f, 5.f);
    
    initCircle(&pGame->circle, &basicMaterial, 16, 5.f);
    transformTranslate(&pGame->circle.transform, 6.f, 2.f, 0.f);
    
    initSprite(&pGame->sprite, &textureMaterial, pSpritePath, squareVertices, squareUV);
    transformTranslate(&pGame->sprite.transform, -6.f, 0.f, 0.f);
    transformScale(&pGame->sprite.transform, 6.f, 6.f, 6.f);
    
    initAnimation(&pGame->anim, &ss, frames, ArrayCount(frames));
#endif
    
#if 0    
    initSpriteSheet(&pGame->ss, &textureMaterial, pSpriteSheetPath, 
                    squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->ss, 64);
    
    initCharacter(&pGame->character1, pCM);
    initCharacter(&pGame->character2, pCM);
    initCharacter(&pGame->character3, pCM);
    moveCharacter(&pGame->character1, 20.f, -9.f);
    moveCharacter(&pGame->character2, 9.f, 0.f);
    moveCharacter(&pGame->character3, 0.f, -7.f);
    pGame->character2.pCollider->mass = 5.f;
    
	char* pTilesetPath = "..//resources//tileset.png";
	initSpriteSheet(&pGame->tileset, &textureMaterial, pTilesetPath, squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->tileset, 32);
    
    initTilemap(&pGame->tilemap, "..//resources//tilemap.csv", 
                &pGame->tileset, &textureMaterial, pRenderer);
    s32 collisionableTiles[] = { 37, 45, 46, 47, 55 };
	tilemapSetCollisionableTiles(&pGame->tilemap, 
                                 collisionableTiles, 
                                 sizeof(collisionableTiles) / sizeof(s32));
    
	tilemapRegisterColliders(&pGame->tilemap, pGame->character1.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->character2.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->character3.pCollider);
#endif
    
    pGame->timer = {};
}

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    //transformTranslate(&pGame->sprite.transform, .01f, 0.f, 0.f);
    //transformTranslate(&pGame->cs.transform, -.01f, 0f, 0.f);
    //drawTriangle(&pGame->triangle, pRenderer);
    //drawColorSquare(&pGame->cs, pRenderer);
    //drawCircle(&pGame->circle, pRenderer);
    //drawSprite(&pGame->sprite, pRenderer);
    //drawSprite(&pGame->ss.sprite, pRenderer);
    //updateAnimation(&pGame->anim, deltaTime);
    //transformRotateY(&pGame->cs.transform, 5.f);
    //transformRotate(&pGame->sprite.transform, 5.f, );
    pGame->timer += pTime->deltaTime;
#if 1
    if (pGame->timer >= 3.f) {
        pRenderer->pCamera->fov += 10.f;
        pGame->timer = 0.f;
    }
    if (pGame->camX > 32.f) {
        pGame->camX = 0.f;
        pGame->camY -= 5.f;
        if (pGame->camY < -35.f) {
            pGame->camY = 0.f;
        }
    } else {
        pGame->camX += 2.f  * pTime->deltaTime;
    }
    
#endif
    //pRenderer->pCamera->position = HMM_Vec3(pGame->camX, pGame->camY, pRenderer->pCamera->position.z);
    if (isKeyPressed(pRenderer, KEY_D)) {
        moveCamera(&pGame->camera, pRenderer->pCamera->right, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_A)) {
        moveCamera(&pGame->camera, -1.f * pRenderer->pCamera->right, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_W)) {
        moveCamera(&pGame->camera, pRenderer->pCamera->front, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_S)) {
        moveCamera(&pGame->camera, -1.f * pRenderer->pCamera->front, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_E)) {
        moveCamera(&pGame->camera, pRenderer->pCamera->up, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_Q)) {
        moveCamera(&pGame->camera, -1.f * pRenderer->pCamera->up, pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_Z)) {
        pRenderer->pCamera->roll -= pTime->deltaTime * 5.f;
        pRenderer->pCamera->roll -= pTime->deltaTime * 5.f;
        updateCameraVectors(pRenderer->pCamera);
    }
    if (isKeyPressed(pRenderer, KEY_C)) {
        pRenderer->pCamera->roll += pTime->deltaTime * 5.f;
        updateCameraVectors(pRenderer->pCamera);
    }
#if 0
    transformRotate(&pGame->character1.transform, 1.f, VEC3_X);
    //setCameraPosition(pRenderer, pGame->camX, pGame->camY);
    drawTilemap(&pGame->tilemap, pRenderer);
    moveCharacter(&pGame->character1, -2.f * pTime->deltaTime, 0.f);
    updateCharacter(&pGame->character1, pTime->deltaTime);
    drawCharacter(&pGame->character1, pRenderer);
    moveCharacter(&pGame->character2, 0.f, -2.f * pTime->deltaTime);
    updateCharacter(&pGame->character2, pTime->deltaTime);
    drawCharacter(&pGame->character2, pRenderer);
    moveCharacter(&pGame->character3, 2.f * pTime->deltaTime, 0.f);
    updateCharacter(&pGame->character3, pTime->deltaTime);
    drawCharacter(&pGame->character3, pRenderer);
    tilemapCheckCollisions(&pGame->tilemap);
#endif
    drawModel(&pGame->model, pRenderer);
    drawModel(&pGame->model2, pRenderer);
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}