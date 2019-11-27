Complex
getTwiddle(u32 x, u32 size) {
    f64 angle = -(2.*PI32*x)/(f64)size;
    f64 cosValue = cos(angle);
    f64 sinValue = sin(angle);
    //return { cosValue, 1.f - cosValue*cosValue };
    return { cosValue, sinValue };
}

void
computeSetTwiddle(FourierData* pFourierData) {
	bindBuffer(pFourierData->twiddleSSBO, SHADER_STORAGE_BUFFER);
    
    u32 halfSize = SAMPLE_RATE/2;
    
    for (u32 i = 0; i < halfSize; ++i) {
        pFourierData->pTwiddle[i] = getTwiddle(i, SAMPLE_RATE);
    }
    for (u32 i = 0; i < halfSize; ++i) {
        pFourierData->pTwiddle[i + halfSize] = -pFourierData->pTwiddle[i];
    }
}

void
initFourierData(FourierData* pFourierData) {
    pFourierData->inputSSBO = initBuffer(SHADER_STORAGE_BUFFER, SAMPLE_RATE * sizeof(Complex));
    pFourierData->twiddleSSBO = initBuffer(SHADER_STORAGE_BUFFER, SAMPLE_RATE * sizeof(Complex));
    s32 dataBuffer = initBuffer(SHADER_STORAGE_BUFFER, VISUALIZER_BAND_BUFFER * sizeof(f32));
    initComputeShader(&pFourierData->computeShader,
                      "..//resources//shaders//cMusicVisualizer.glsl", 
                      1096);
    
    pFourierData->pReversed = (u32*)malloc(sizeof(u32)*SAMPLE_RATE);
    pFourierData->pWindow = (f32*)malloc(sizeof(f32)*SAMPLE_RATE);
    pFourierData->pTwiddle = (Complex*)malloc(sizeof(Complex)*SAMPLE_RATE);
    
    f32 constant = PI32/SAMPLE_RATE;
    for (u32 i = 0; i < SAMPLE_RATE; ++i) {
        pFourierData->pWindow[i] = sinf(constant*i);
    }
    for (u32 i = 0; i < SAMPLE_RATE; ++i) {
        pFourierData->pReversed[i] = reverseBits(i, SAMPLE_RATE);
    }
    
    computeSetTwiddle(pFourierData);
    memoryBarrier();
    
    shaderBindID(pFourierData->computeShader.id);
    shaderSetInt(&pFourierData->computeShader, "stride", STRIDE);
    shaderSetInt(&pFourierData->computeShader, "timeSize", TIME_SIZE);
    shaderSetInt(&pFourierData->computeShader, "size", SAMPLE_RATE);
    u32 size = sizeof(Complex)*(SAMPLE_RATE);
    bindBuffer(pFourierData->twiddleSSBO, SHADER_STORAGE_BUFFER);
    bufferDataDraw(SHADER_STORAGE_BUFFER, size);
    Complex* pFInput = (Complex*)mapBufferRangeWrite(SHADER_STORAGE_BUFFER, 
                                                     0, size);
    memcpy(pFInput, pFourierData->pTwiddle, size);
    unmapBuffer(SHADER_STORAGE_BUFFER);
    bindBufferBase(pFourierData->inputSSBO, 4);
    bindBufferBase(pFourierData->twiddleSSBO, 5);
    bindBufferBase(dataBuffer, 6);
}

void
setFourierInputAndCalculate(FourierData* pFourierData, f32* pData) {
    bindBuffer(pFourierData->inputSSBO, SHADER_STORAGE_BUFFER);
    bufferDataDraw(SHADER_STORAGE_BUFFER, SAMPLE_RATE*sizeof(Complex));
    
    Complex* pFInput = (Complex*)mapBufferRangeWrite(SHADER_STORAGE_BUFFER, 
                                                     0, SAMPLE_RATE * sizeof(Complex));
    for (u32 iData = 0; iData < SAMPLE_RATE; ++iData) {
        pData[iData] *= pFourierData->pWindow[iData];
    }
    for (u32 iData = 0; iData < SAMPLE_RATE; ++iData) {
        pFInput[iData].r = pData[pFourierData->pReversed[iData]];
        pFInput[iData].i = 0.f;
    }
    
    unmapBuffer(SHADER_STORAGE_BUFFER);
    shaderBindID(pFourierData->computeShader.id);
    memoryBarrier();
    dispatch(SAMPLE_RATE/32, 1, 1);
    memoryBarrier();
}

void
initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Shader* pShader) {
    
    pMusicVisualizerConfig->pShader = pShader;
    pMusicVisualizerConfig->pBandValues = pMusicData->pTBuffer;
    pMusicVisualizerConfig->bandAmount = (SAMPLE_RATE/2)/STRIDE;
    pMusicVisualizerConfig->stride = STRIDE;
    pMusicVisualizerConfig->timeSize = TIME_SIZE;
    initVA(&pMusicVisualizerConfig->va);
    vaBind(pMusicVisualizerConfig->va);
    
    pMusicData->pFourierData = &pMusicVisualizerConfig->fourierData;
    
    initFourierData(&pMusicVisualizerConfig->fourierData);
    
    shaderBindID(pMusicVisualizerConfig->pShader->id);
    
    b32 vaporWave = false;
    b32 blackBorder = false;
    
    hmm_vec4 colorLFN;
    hmm_vec4 colorHFN;
    hmm_vec4 colorLFP;
    hmm_vec4 colorHFP;
    hmm_vec4 borderColor;
    if (vaporWave) {
        //fillColor(.075f, 0.f, .15f);
        fillColor(.8f, .9f, 1.f);
        colorLFN = { 1.f, .7f, 1.f, 1.f };
        colorHFN = { .6f, 1.f, 1.f, 1.f };
        colorLFP = { .5f, .2f, .5f, 1.f };
        colorHFP = { .1f, .5f, .5f, 1.f };
        shaderSetBool(pMusicVisualizerConfig->pShader, "hasFakeTransparency", false);
    } else {
        fillColor(0.f, 0.f, 0.f);
        colorLFN = { 1.f, 0.f, 0.f, 1.f };
        colorHFN = { 1.f, 1.f, 0.f, 1.f };
        colorLFP = { 0.f, 0.f, 1.f, 1.f };
        colorHFP = { 0.f, 1.f, 0.f, 1.f };
        shaderSetBool(pMusicVisualizerConfig->pShader, "hasFakeTransparency", true);
    }
    if (blackBorder) {
        borderColor = { 0.f, 0.f, 0.f, 1.f };
    } else {
        borderColor = { 1.f, 1.f, 1.f, 1.f };
    }
    shaderSetVec4(pMusicVisualizerConfig->pShader, "colorLFN", &colorLFN); 
    shaderSetVec4(pMusicVisualizerConfig->pShader, "colorHFN", &colorHFN);
    shaderSetVec4(pMusicVisualizerConfig->pShader, "colorLFP", &colorLFP); 
    shaderSetVec4(pMusicVisualizerConfig->pShader, "colorHFP", &colorHFP);
    shaderSetVec4(pMusicVisualizerConfig->pShader, "borderColor", &borderColor);
    
    shaderSetFloat(pMusicVisualizerConfig->pShader, "height", 15.f);
    shaderSetInt(pMusicVisualizerConfig->pShader, "stride", pMusicVisualizerConfig->stride);
    shaderSetInt(pMusicVisualizerConfig->pShader, "bandAmount", pMusicVisualizerConfig->bandAmount);
    shaderSetInt(pMusicVisualizerConfig->pShader, "timeSize", pMusicVisualizerConfig->timeSize);
}

void
drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Renderer* pRenderer) {
    
    if (pMusicData->changed) {
        setFourierInputAndCalculate(&pMusicVisualizerConfig->fourierData, pMusicData->pTBuffer);
        pMusicData->changed = false;
    }
    
    shaderBindID(pMusicVisualizerConfig->pShader->id);
    hmm_mat4 viewProj = getViewProj(pRenderer);
    shaderSetMat4(pMusicVisualizerConfig->pShader,
                  "viewProj",
                  &viewProj);
    vaBind(pMusicVisualizerConfig->va);
    setDepthTestState(false);
    shaderSetBool(pMusicVisualizerConfig->pShader, "isBorder", true);
    shaderSetFloat(pMusicVisualizerConfig->pShader, "scale", 1.1f);
    drawPointsInstanced(0, 1, VISUALIZER_BAND_BUFFER);
    shaderSetBool(pMusicVisualizerConfig->pShader, "isBorder", false);
    shaderSetFloat(pMusicVisualizerConfig->pShader, "scale", 1.f);
    if (!isKeyPressed(pRenderer, KEY_UP)) {
        setDepthTestState(true);
    }
    drawPointsInstanced(0, 1, VISUALIZER_BAND_BUFFER);
}
