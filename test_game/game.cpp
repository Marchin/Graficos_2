internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->timer = {};
    
    Material modelMaterial;
    initMaterial(&modelMaterial, "Model", "..//resources//shaders//vModel.glsl", 
                 "..//resources//shaders//fModel.glsl");
    
    initCharacter(&pGame->scene, "../resources/scene.fbx");
    initCharacter(&pGame->cubeE, "../resources/cube.obj");
    
    addChild(&pGame->cubeE.transform, &pGame->scene.transform);
    transformTranslate(&pGame->cubeE.transform, 0.f, 0.65f, 0.f);
    
#if 1
    generateWalls(&pGame->scene.transform, &pGame->level);
#endif
}

internal s32 counter;

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->timer += pTime->deltaTime;
    ++counter;
    
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
    
    // Cube Movement
    if (isKeyPressed(pRenderer, KEY_UP)) {
        transformTranslate(&pGame->cubeE.transform, 0.f, 0.f, -2.f*pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_DOWN)) {
        transformTranslate(&pGame->cubeE.transform, 0.f, 0.f, 2.f*pTime->deltaTime);
    }
    if (isKeyPressed(pRenderer, KEY_LEFT)) {
        transformTranslate(&pGame->cubeE.transform, -2.f*pTime->deltaTime, 0.f, 0.f);
    }
    if (isKeyPressed(pRenderer, KEY_RIGHT)) {
        transformTranslate(&pGame->cubeE.transform, 2.f*pTime->deltaTime, 0.f, 0.f);
    }
    
    transformUpdate(&pGame->scene.transform, pTime->deltaTime);
    checkBSPPlanes(&pGame->scene.transform, pRenderer, &pGame->level);
    transformDraw(&pGame->scene.transform, pRenderer);
    
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}