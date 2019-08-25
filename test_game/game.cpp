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
    
    initTransform(&pGame->go.transform);
    pGame->go.transform.pEntity = &pGame->go;
    pGame->go.material = basicMaterial;
    Triangle* pTriangle = (Triangle*)addComponent(TRIANGLE, 
                                                  &pGame->go.transform);
    initTriangle(pTriangle, &pGame->go.transform, &pGame->go.material, 
                 &vertices, sizeof(vertices));
    
    Circle* pCircle = (Circle*)addComponent(CIRCLE, 
                                            &pGame->go.transform);
    initCircle(pCircle, &pGame->go.transform, &pGame->go.material, 
               16, .5f);
    
#if 0
    initModel(&pGame->model, "../resources/mesa_rol/MesaRol.obj", &modelMaterial);
    //initModel(&pGame->model, "../resources/cube.obj", &modelMaterial);
    initModel(&pGame->model2, "../resources/bath.obj", &modelMaterial);
    freeModel(&pGame->model);
    
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
    
    initSpriteSheet(&pGame->ss, &textureMaterial, pSpriteSheetPath, 
                    squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->ss, 64);
#endif
    
    initFigure(&pGame->figure1, pCM);
    initFigure(&pGame->figure2, pCM);
    initFigure(&pGame->figure3, pCM);
    moveFigure(&pGame->figure1, 2.f, -9.f);
    moveFigure(&pGame->figure2, 1.f, 0.f);
    moveFigure(&pGame->figure3, 0.f, -7.f);
    pGame->figure2.pCollider->mass = 5.f;
#if 1    
    
	char* pTilesetPath = "..//resources//tileset.png";
	initSpriteSheet(&pGame->tileset, &pGame->tilemap.transform,
                    &textureMaterial, pTilesetPath, squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->tileset, 32);
    
    initTilemap(&pGame->tilemap, "..//resources//tilemap.csv", 
                &pGame->tileset, &textureMaterial, pRenderer);
    s32 collisionableTiles[] = { 37, 45, 46, 47, 55 };
	tilemapSetCollisionableTiles(&pGame->tilemap, 
                                 collisionableTiles, 
                                 sizeof(collisionableTiles) / sizeof(s32));
    
	tilemapRegisterColliders(&pGame->tilemap, pGame->figure1.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->figure2.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->figure3.pCollider);
#endif
    
    initCharacter(&pGame->character, "../resources/bath.obj");
    
    initTransform(&pGame->scene);
    addChild(&pGame->figure1.transform, &pGame->scene);
    addChild(&pGame->figure2.transform, &pGame->figure1.transform);
    addChild(&pGame->figure3.transform, &pGame->scene);
    addChild(&pGame->go.transform, &pGame->scene);
    addChild(&pGame->character.transform, &pGame->scene);
    removeChild(&pGame->figure2.transform);
    pGame->timer = {};
    pGame->camera.projectionType = ORTHOGRAPHIC;
    transformTranslate(&pGame->character.transform, 0.f, 0.f, -60.f);
}

local_persist s32 counter;

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    //transformTranslate(&pGame->sprite.transform, .01f, 0.f, 0.f);
    //transformTranslate(&pGame->cs.transform, -.01f, 0f, 0.f);
    //drawTriangle(pGame->pTriangle, pRenderer);
    //drawColorSquare(&pGame->cs, pRenderer);
    //drawCircle(&pGame->circle, pRenderer);
    //drawSprite(&pGame->sprite, pRenderer);
    //drawSprite(&pGame->ss.sprite, pRenderer);
    //updateAnimation(&pGame->anim, deltaTime);
    //transformRotateY(&pGame->cs.transform, 5.f);
    //transformRotate(&pGame->sprite.transform, 5.f, );
    pGame->timer += pTime->deltaTime;
    counter++;
    
#if 0
    float vertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.f, 0.f
	};
    if (counter == 200) {
        removeComponent(TRIANGLE, pGame->go.transform.pComponents, 4);
    } else if (counter == 300) {
        Triangle* pTriangle = (Triangle*)addComponent(TRIANGLE, 
                                                      &pGame->go.transform);
        initTriangle(pTriangle, &pGame->go.transform, &pGame->go.material, 
                     &vertices, sizeof(vertices));
    }
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
    drawTilemap(&pGame->tilemap, pRenderer);
    moveFigure(&pGame->figure1, -2.f * pTime->deltaTime, 0.f);
    transformRotate(&pGame->figure1.transform, 1.f, VEC3_Y);
    transformRotate(&pGame->figure2.transform, 5.f, VEC3_Y);
    transformRotate(&pGame->go.transform, 5.f, VEC3_Y);
    transformTranslate(&pGame->figure3.transform, 0.f, 0.01f, .01f);
    //moveFigure(&pGame->figure2, 0.f, -2.f * pTime->deltaTime);
    //moveFigure(&pGame->figure3, 2.f * pTime->deltaTime, 0.f);
    transformRotate(&pGame->figure1.transform, 1.f, VEC3_X);
    //setCameraPosition(pRenderer, pGame->camX, pGame->camY);
    updateFigure(&pGame->figure1, pTime->deltaTime);
    drawFigure(&pGame->figure1, pRenderer);
    updateFigure(&pGame->figure2, pTime->deltaTime);
    drawFigure(&pGame->figure2, pRenderer);
    updateFigure(&pGame->figure3, pTime->deltaTime);
    drawFigure(&pGame->figure3, pRenderer);
    transformRotate(&pGame->model2.transform, 1.f, VEC3_X);
    drawModel(&pGame->model, pRenderer);
    drawModel(&pGame->model2, pRenderer);
#endif
    tilemapCheckCollisions(&pGame->tilemap);
    transformUpdate(&pGame->scene, pTime->deltaTime);
    transformDraw(&pGame->scene, pRenderer);
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}