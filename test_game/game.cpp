
internal char*
readSongPath() {
    FILE* pFile = fopen("..//build//Song.txt", "r");
    char* pMusicPath = 0;
    
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        u32 size = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);
        pMusicPath = (char*)malloc(size + 1);
        if (pMusicPath && size) {
            fread(pMusicPath, sizeof(char), size, pFile);
        }
        while (pMusicPath[size - 1] < '\0' && size > 0) {
            --size; // NOTE(Marchin): for some reason some text files have 
            //non-valid characters at EOF, so we get rid of them
        }
        pMusicPath[size] = '\0'; 
        fclose(pFile);
    } else {
        perror("Error opening file\n");
        pFile = fopen("..//build//Song.txt", "w");
        if (pFile) {
            fprintf(pFile, "../resources/Enter Sandman.wav");
        }
        pMusicPath = "../resources/Enter Sandman.wav";
    }
    
    return pMusicPath;
}

internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
#if 0
    initShader(&pGame->musicVisualizerShader, "MusicVisualizer",
               "..//resources//shaders//vMusicVisualizer.glsl", 
               "..//resources//shaders//fMusicVisualizer.glsl", 
               "..//resources//shaders//gMusicVisualizer.glsl");
    
    char* wavPath = readSongPath();
    //readWAV(&pGame->sound,"../resources/bass.wav");
    //readWAV(&pGame->sound,"../resources/test.wav");
    //readWAV(&pGame->sound,"../resources/moonglow.wav");
    //readWAV(&pGame->sound,"../resources/Hold The Line.wav");
    //readWAV(&pGame->sound,"../resources/A Horse With No Name.wav");
    if (readWAV(&pGame->sound, wavPath) != 0) {
        FILE* pFile = fopen("..//build//Song.txt", "w");
        if (pFile) {
            fprintf(pFile, "../resources/Enter Sandman.wav");
        }
        fclose(pFile);
        readWAV(&pGame->sound,"../resources/Hold The Line.wav");
    }
    
    initMusicData(&pGame->musicData, &pGame->sound);
    
    pGame->musicVisualizerConfig = {};
    initMusicVisualizer(&pGame->musicVisualizerConfig,
                        &pGame->musicData,
                        &pGame->musicVisualizerShader);
    
    s32 err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    }
    
    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream,
                               0,
                               pGame->sound.channels,
                               paInt16,
                               pGame->sound.sampleRate,
                               SAMPLE_RATE,
                               fftCallback,
                               &pGame->musicData);
    
    err = Pa_StartStream(stream);
#endif
    fillColor(.2f, .2f, .2f);
    
    Rotor3 a = getRotorFromMat4(HMM_Mat4d(1.0f));
    
    initShader(&pGame->modelShader, "Model", "..//resources//shaders//vModel.glsl", 
               "..//resources//shaders//fModel.glsl"); 
    initTransform(&pGame->empty);
    initCharacter(&pGame->character, "..//resources//bath.obj");
    initCharacter(&pGame->character2, "..//resources//bath.obj");
    initCharacter(&pGame->character3, "..//resources//bath.obj");
    initCharacter(&pGame->character4, "..//resources//bath.obj");
    
    //parseModel(&pGame->modelData, "..//resources//bsp2.obj");
    //parseCollada(&pGame->modelStructure, "..//resources//cube.dae");
    parseCollada(&pGame->modelStructure, "..//resources//bsp.dae");
    //parseCollada(&pGame->modelStructure, "..//resources//bsp6.dae");
    //parseModel(&pGame->modelStructure, "..//resources//cube.obj");
    //initParsedModel(&pGame->modelStructure, &pGame->modelShader);
    
    addChild(&pGame->modelStructure.transform, &pGame->empty);
#if 0
    addChild(&pGame->character.transform, &pGame->empty);
    addChild(&pGame->character2.transform, &pGame->empty);
    pGame->character2.transform.position = {-0.f, 0.f, 40.f};
    addChild(&pGame->character3.transform, &pGame->empty);
    pGame->character3.transform.position = {-20.f, 0.f, 20.f};
    addChild(&pGame->character4.transform, &pGame->empty);
    pGame->character4.transform.position = {20.f, 0.f, 20.f};
#endif
}

global b32 gPaused;
global b32 gPrevPaused;
global u32 gPauseCounter;
global u32 gBlock = true;

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
#if 0
    drawMusicVisualizer(&pGame->musicVisualizerConfig, &pGame->musicData, pRenderer);
    gPrevPaused = gPaused;
    if (isKeyPressed(pRenderer, KEY_SPACE)) {
        if (!gBlock) {
            ++gPauseCounter;
            if (gPauseCounter == 4){
                gPaused = true;
                gBlock = true;
            }
            if (gPaused && !gPrevPaused) {
                pGame->musicData.paused = !pGame->musicData.paused;
            }
        }
    } else {
        gPauseCounter = 0;
        gPaused = false;
        gBlock = false;
    }
#endif
    //drawParsedModel(&pGame->modelData);
    V3 up = getRotatedVector(VEC3_Y, pGame->character.transform.rotor);
    V3 right = getRotatedVector(VEC3_X, pRenderer->pCamera->transform.rotor);
    V3 front = getRotatedVector(VEC3_Z, pRenderer->pCamera->transform.rotor);
    if (isKeyPressed(pRenderer, KEY_D)) {
        moveCamera(&pGame->camera, 10.f * right, pTime->deltaTime);
        //transformRotate(&pGame->character.transform, 30.0f*pTime->deltaTime, up);
    }
    if (isKeyPressed(pRenderer, KEY_A)) {
        moveCamera(&pGame->camera, -10.f * right, pTime->deltaTime);
        //transformRotate(&pGame->character.transform, -30.0f*pTime->deltaTime, up);
    }
    if (isKeyPressed(pRenderer, KEY_W)) {
        moveCamera(&pGame->camera, -10.f * front, pTime->deltaTime);
        //transformRotate(&pGame->character.transform, 30.0f*pTime->deltaTime, right);
    }
    if (isKeyPressed(pRenderer, KEY_S)) {
        moveCamera(&pGame->camera, 10.f * front, pTime->deltaTime);
        //transformRotate(&pGame->character.transform, -30.0f*pTime->deltaTime, right);
    }
    if (isKeyPressed(pRenderer, KEY_E)) {
        //moveCamera(&pGame->camera, up, pTime->deltaTime);
        transformRotate(&pGame->character.transform, 30.0f*pTime->deltaTime, front);
    }
    if (isKeyPressed(pRenderer, KEY_Q)) {
        //moveCamera(&pGame->camera, -1.f * up, pTime->deltaTime);
        transformRotate(&pGame->character.transform, -30.0f*pTime->deltaTime, front);
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
    
#if 1
    const hmm_mat4 diag = HMM_Mat4d(1.f);
    transformUpdate(&pGame->empty, pTime->deltaTime, diag);
    transformDraw(&pGame->empty, pRenderer, diag);
#endif
    
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}
