internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->timer = {};
    
    Material modelMaterial;
    initMaterial(&modelMaterial, "Model", "..//resources//shaders//vModel.glsl", 
                 "..//resources//shaders//fModel.glsl");
    
    initTransform(&pGame->scene);
    strcpy(pGame->scene.name, "scene");
    initCharacter(&pGame->floor, "../resources/cube.obj");
    initCharacter(&pGame->cubeA, "../resources/cube.obj");
    initCharacter(&pGame->cubeB, "../resources/cube.obj");
    initCharacter(&pGame->cubeC, "../resources/cube.obj");
    initCharacter(&pGame->cubeD, "../resources/cube.obj");
    initCharacter(&pGame->cubeE, "../resources/cube.obj");
    initCharacter(&pGame->wall1, "../resources/cube.obj");
    initCharacter(&pGame->wall2, "../resources/cube.obj");
    initCharacter(&pGame->wall3, "../resources/cube.obj");
    
    addChild(&pGame->floor.transform, &pGame->scene);
    addChild(&pGame->cubeA.transform, &pGame->scene);
    addChild(&pGame->cubeB.transform, &pGame->scene);
    addChild(&pGame->cubeC.transform, &pGame->scene);
    addChild(&pGame->cubeD.transform, &pGame->scene);
    addChild(&pGame->cubeE.transform, &pGame->scene);
    addChild(&pGame->wall1.transform, &pGame->scene);
    addChild(&pGame->wall2.transform, &pGame->scene);
    addChild(&pGame->wall3.transform, &pGame->scene);
    transformTranslate(&pGame->floor.transform, 0.f, -1.f, -10.f);
    transformTranslate(&pGame->cubeA.transform, -5.f, 0.f, -5.f);
    transformTranslate(&pGame->cubeB.transform, 0.f, 0.f, -5.f);
    transformTranslate(&pGame->cubeC.transform, 5.f, 0.f, -5.f);
    transformTranslate(&pGame->cubeD.transform, 0.f, 0.f, -15.f);
    transformTranslate(&pGame->cubeE.transform, 0.f, 0.f, 0.f);
    transformTranslate(&pGame->wall1.transform, 4.f, 0.f, -10.f);
    transformTranslate(&pGame->wall2.transform, -4.f, 0.f, -10.f);
    transformTranslate(&pGame->wall3.transform, 0.f, 0.f, -10.f);
    transformScale(&pGame->floor.transform, 20.f, 0.5f, 20.f);
    transformScale(&pGame->wall1.transform, 0.5f, 3.f, 10.f);
    transformScale(&pGame->wall2.transform, 0.5f, 3.f, 10.f);
    transformScale(&pGame->wall3.transform, 10.f, 3.f, 0.5f);
    
#if 1
    pGame->level.bspPlaneCount = 3;
    pGame->level.pBSPPlanes = (Plane*)malloc(sizeof(Plane)*pGame->level.bspPlaneCount);
    Plane* pPlane = pGame->level.pBSPPlanes;
    pPlane->normal = {1.f, 0.f, 0.f};
    pPlane->dot = {4.f, 0.f, 0.f};
    pPlane->d = -HMM_DotVec3(pPlane->normal, pPlane->dot);
    ++pPlane;
    pPlane->normal = {1.f, 0.f, 0.f};
    pPlane->dot = {-4.f, 0.f, 0.f};
    pPlane->d = -HMM_DotVec3(pPlane->normal, pPlane->dot);
    ++pPlane;
    pPlane->normal = {0.f, 0.f, 1.f};
    pPlane->dot = {0.f, 0.f, -10.f};
    pPlane->d = -HMM_DotVec3(pPlane->normal, pPlane->dot);
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
    
    transformUpdate(&pGame->scene, pTime->deltaTime);
    checkBSPPlanes(&pGame->scene, pRenderer, &pGame->level);
    transformDraw(&pGame->scene, pRenderer);
    
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}