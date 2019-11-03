internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->timer = {};
    
    Material modelMaterial;
    initMaterial(&modelMaterial, "Model", "..//resources//shaders//vModel.glsl", 
                 "..//resources//shaders//fModel.glsl");
    
    initCharacter(&pGame->scene, "../resources/bsp.fbx");
    initCharacter(&pGame->cubeE, "../resources/cube.obj");
    
    initMaterial(&pGame->musicVisualizerMaterial, "MusicVisualizer",
                 "..//resources//shaders//vMusicVisualizer.glsl", 
                 "..//resources//shaders//fMusicVisualizer.glsl");
    
    //addChild(&pGame->cubeE.transform, &pGame->scene.transform);
    transformTranslate(&pGame->cubeE.transform, 0.f, 0.65f, 0.f);
    //readWAV(&pGame->sound,"../resources/test.wav");
    //readWAV(&pGame->sound,"../resources/moonglow.wav");
    readWAV(&pGame->sound,"../resources/Hold The Line.wav");
    //readWAV(&pGame->sound,"../resources/A Horse With No Name.wav");
    generateWalls(&pGame->scene.transform, &pGame->level);
    
    pGame->musicVisualizerConfig = {};
    initMusicVisualizer(&pGame->musicVisualizerConfig, &pGame->musicVisualizerMaterial);
    
    s32 err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    }
    
    setMusicData(pGame->sound.block_align, pGame->sound.data_size);
    
    
    PaStream *stream;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream(&stream,
                               0,/* no input channels */
                               pGame->sound.channels,// stereo output 
                               paInt16,  /* 32 bit floating point output */
                               pGame->sound.sample_rate,
                               SAMPLE_RATE, /* frames per buffer, i.e. the number
                               of sample frames that PortAudio will
                               request from the callback. Many apps
                               may want to use
                               paFramesPerBufferUnspecified, which
                               tells PortAudio to pick the best,
                               possibly changing, buffer size.*/
                               fftCallback, /* this is your callback function */
                               pGame->sound.pData); /*This is a pointer that will be passed to your callback*/
    
#if 0
    err = Pa_OpenDefaultStream(&stream,
                               0,/* no input channels */
                               1,// stereo output 
                               paFloat32,  /* 32 bit floating point output */
                               pGame->sound.sample_rate,
                               FRAMES_PER_BUFFER, /* frames per buffer, i.e. the number
                               of sample frames that PortAudio will
                               request from the callback. Many apps
                               may want to use
                               paFramesPerBufferUnspecified, which
                               tells PortAudio to pick the best,
                               possibly changing, buffer size.*/
                               sinCallback, /* this is your callback function */
                               0); /*This is a pointer that will be passed to your callback*/
#endif
    
    err = Pa_StartStream(stream);
    
    f32 value = {};
#if 0
    f32 wave_period = 512.f/44100.f;
    for (u32 i = 0; i < NUM; ++i) {
        out->r= sinf(2*PI32*value);
        out++;
        value += wave_period;
        if (value >= 1.f) {
            value = 1.f;
            wave_period = -wave_period;
        }
    }
    for (u32 i = 0; i < NUM; ++i) {
        out->r = sinf(value*PI32);
        out++;
        value += 0.005;
        if (value > 2.f) {
            value = 0;
        }
    }
    
    pGame->mycfg = kiss_fft_alloc(NUM, 0, NULL, NULL);
    kiss_fft_cpx buffer[NUM] = {};
    kiss_fft_cpx* out = buffer;
    
    kiss_fft_cpx output[NUM];
    kiss_fft(pGame->mycfg, buffer, output);
    f32 mod[NUM] = {};
    for (u32 i = 0; i < NUM; ++i) {
        mod[i] = sqrt(output[i].r*output[i].r + output[i].i*output[i].i);
        
    }
#endif
}

internal s32 counter;
global b32 gPaused;
global b32 gPrevPaused;
global u32 gPauseCounter;
global u32 gBlock = true;

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->timer += pTime->deltaTime;
    ++counter;
    
    drawMusicVisualizer(&pGame->musicVisualizerConfig, pRenderer);
    
    gPrevPaused = gPrevPaused;
    if (isKeyPressed(pRenderer, KEY_SPACE)) {
        if (!gBlock) {
            ++gPauseCounter;
            if (gPauseCounter == 4){
                gPaused = true;
                gBlock = true;
            }
            if (gPaused && !gPrevPaused) {
                playPauseMusic();
            }
        }
    } else {
        gPauseCounter = 0;
        gPaused = false;
        gBlock = false;
    }
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
    
    transformUpdate(&pGame->empty, pTime->deltaTime);
#if 0
    checkBSPPlanes(&pGame->scene.transform, pRenderer, &pGame->level);
#endif
    transformDraw(&pGame->empty, pRenderer);
    
    f64 x, y;
    getMousePos(pRenderer->pWindow, &x, &y);
    cameraMouseMovement(pRenderer->pCamera, x, y, true);
}

