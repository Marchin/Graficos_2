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
	bindBuffer(pFourierData->twiddleSSBO, GL_SHADER_STORAGE_BUFFER);
    
    u32 halfSize = SAMPLE_RATE/2;
#if 0
    for (u32 i = 0; i < SAMPLE_RATE; ++i) {
        pFourierData->pTwiddle[i] = getTwiddle(i, SAMPLE_RATE);
    }
#else
    for (u32 i = 0; i < halfSize; ++i) {
        pFourierData->pTwiddle[i] = getTwiddle(i, SAMPLE_RATE);
    }
    for (u32 i = 0; i < halfSize; ++i) {
        pFourierData->pTwiddle[i + halfSize] = -pFourierData->pTwiddle[i];
    }
#endif
}

void
initFourierData(FourierData* pFourierData) {
    pFourierData->inputSSBO = initBuffer(GL_SHADER_STORAGE_BUFFER, SAMPLE_RATE * sizeof(Complex));
    pFourierData->twiddleSSBO = initBuffer(GL_SHADER_STORAGE_BUFFER, SAMPLE_RATE * sizeof(Complex));
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
#if 0
    char uniformParamaterName[16] = "reversed[";
    u32 nameSize = arrayCount(uniformParamaterName);
    char closedBraket[2] = "]";
    char num[5];
    for (u32 i = 0; i < SAMPLE_RATE; ++i) {
        _itoa(i, num, 10);
        strcat(uniformParamaterName, num);
        strcat(uniformParamaterName, closedBraket);
        shaderSetInt(&pFourierData->computeShader,
                     uniformParamaterName,
                     pFourierData->pReversed[i]);
        uniformParamaterName[9] = 0;
    }
#endif
    
    computeSetTwiddle(pFourierData);
    glCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
    
    shaderBindID(pFourierData->computeShader.id);
    u32 size = sizeof(Complex)*(SAMPLE_RATE);
    bindBuffer(pFourierData->twiddleSSBO, GL_SHADER_STORAGE_BUFFER);
    glCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STREAM_DRAW));
    glCall(Complex* pFInput = (Complex*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 
                                                         0, size, 
                                                         GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
    memcpy(pFInput, pFourierData->pTwiddle, size);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    bindBufferBase(pFourierData->inputSSBO, 4);
    bindBufferBase(pFourierData->twiddleSSBO, 5);
}

void
setFourierInputAndCalculate(FourierData* pFourierData, f32* pData) {
    bindBuffer(pFourierData->inputSSBO, GL_SHADER_STORAGE_BUFFER);
    glCall(glBufferData(GL_SHADER_STORAGE_BUFFER, SAMPLE_RATE*sizeof(Complex), NULL, GL_STREAM_DRAW));
    
    Complex* pFInput = (Complex*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 
                                                  0, SAMPLE_RATE * sizeof(Complex), 
                                                  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
#if 0
    f32 Data[] = {
        0.34,	0.69,	0.18,	0.81,
        0.74,	0.32,	0.07,	0.06,
        0.73,	0.49,	0.56,	0.39,
        0.12,	0.69,	0.53,	0.72,
        0.42,	0.24,	0.04,	0.14,
        0.17,	0.90,	0.17,	0.46,
        0.60,	0.46,	0.06,	0.15,
        0.28,	0.13,	0.26,	0.64,
        0.95,	0.20,	0.50,	0.67,
        0.59,	0.65,	0.83,	0.12,
        0.05,	0.83,	0.04,	0.03,
        0.51,	0.36,	0.62,	0.62,
        0.39,	0.85,	0.64,	0.46,
        0.94,	0.28,	0.49,	0.85,
        0.01,	0.06,	0.38,	0.15,
        0.24,	0.17,	0.00,	0.35};
#endif
    
#if 1
    for (u32 iData = 0; iData < SAMPLE_RATE; ++iData) {
        pData[iData] *= pFourierData->pWindow[iData];
    }
#endif
    for (u32 iData = 0; iData < SAMPLE_RATE; ++iData) {
        pFInput[iData].r = pData[pFourierData->pReversed[iData]];
        pFInput[iData].i = 0.f;
    }
    
#if 0
    Complex f[SAMPLE_RATE];
    memcpy(f, pFInput, SAMPLE_RATE*sizeof(Complex));
    int halfPow2 = 1;
    int stride = 2;
    int halfStride = stride/2;
    int halfSize = SAMPLE_RATE/2;
    for (u32 i = 0; i < log2SR; ++i) {
        int offset = SAMPLE_RATE/stride;
        for (int j = 0; j < SAMPLE_RATE; j += stride) {
            int twiddleSelector = 0;
            for (int k = 0; k < halfStride; ++k) {
                int iF = j + k;
                Complex aux = f[iF];
                f[iF] = aux + pFourierData->pTwiddle[twiddleSelector]*f[iF + halfStride];
                f[iF + halfStride] = aux + pFourierData->pTwiddle[twiddleSelector+halfSize]*f[iF + halfStride];
                twiddleSelector += offset;
            }
        }
        halfPow2 = halfStride;
        halfStride = stride;
        stride *= 2;
    }
#endif
    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    shaderBindID(pFourierData->computeShader.id);
    glCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
    glCall(glDispatchCompute(SAMPLE_RATE/32, 1, 1));
    glCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));
    Complex* pFOut = (Complex*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    //Complex* pFOut = f;
    for (u32 iData = 0; iData < SAMPLE_RATE; ++iData) {
        pData[iData] = (f32)sqrt(pFOut[iData].r*pFOut[iData].r +
                                 pFOut[iData].i*pFOut[iData].i);
    }
    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void
initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Shader* pShader) {
    
    pMusicVisualizerConfig->pShader = pShader;
    pMusicVisualizerConfig->pBandValues = pMusicData->pTBuffer;
    pMusicVisualizerConfig->bandCount = VISUALIZER_BANDS;
    initVA(&pMusicVisualizerConfig->va);
    vaBind(pMusicVisualizerConfig->va);
    initVB(&pMusicVisualizerConfig->vb, gCubeVertexData, arrayCount(gCubeVertexData) * sizeof(f32));
    
    pMusicData->pFourierData = &pMusicVisualizerConfig->fourierData;
    
    VertexBufferLayout layout = {};
    u32 layoutsAmount = 1;
    layout.pElements = 
        (VertexBufferElement*)malloc(layoutsAmount*sizeof(VertexBufferElement));
    memset(layout.pElements, 0, layoutsAmount*sizeof(VertexBufferElement));
    layout.elementsMaxSize = layoutsAmount;
    vbLayoutPushFloat(&layout, 3);
    
    vaAddBufferByLocation(pMusicVisualizerConfig->va, 
                          pMusicVisualizerConfig->vb, 
                          &layout, 0);
    free(layout.pElements);
    
#if 1
    initFourierData(&pMusicVisualizerConfig->fourierData);
#endif
    
    shaderBindID(pMusicVisualizerConfig->pShader->id);
    
    b32 vaporWave = true;
    b32 blackBorder = true;
    
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
}

void
drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Renderer* pRenderer) {
    
    
    char uniformParamaterName[11] = "band[";
    u32 nameSize = arrayCount(uniformParamaterName);
    u32 bandCount = pMusicVisualizerConfig->bandCount;
    char closedBraket[2] = "]";
    char num[5];
    //u32 bands[] = { 1, 16, 32, 48, 64, 128, 196, 250 };
    //u32 bands[] = { 4, 16, 20, 24, 32, 48, 64, 96 };
    u32 bands[] = { 0, 6, 8, 12, 14, 16, 20, 24, 36 };
    shaderBindID(pMusicVisualizerConfig->pShader->id);
    
    if (pMusicData->changed) {
#if 1
        setFourierInputAndCalculate(&pMusicVisualizerConfig->fourierData, pMusicData->pTBuffer);
#endif
        shaderBindID(pMusicVisualizerConfig->pShader->id);
        u32 count = VISUALIZER_BAND_BUFFER - bandCount;
#if 1
        for (s32 iBand = count - 1; iBand >= 0; --iBand) {
            pMusicVisualizerConfig->eqBands[iBand + bandCount] =
                pMusicVisualizerConfig->eqBands[iBand];
        }
#endif
        for (s32 iBand = bandCount - 1; iBand >= 0; --iBand) {
            f32 value = pMusicVisualizerConfig->pBandValues[bands[iBand]];
            if (iBand < 3) {
                value *= 0.5f;
                if (iBand == 0) {
                    value *= 0.5f;
                }
            }
            pMusicVisualizerConfig->eqBands[iBand] = value;
        }
        u32 sizesdf = VISUALIZER_BAND_BUFFER;
        for (u32 iBand = 0; iBand < VISUALIZER_BAND_BUFFER; ++iBand) {
            _itoa(iBand, num, 10);
            strcat(uniformParamaterName, num);
            strcat(uniformParamaterName, closedBraket);
            shaderSetFloat(pMusicVisualizerConfig->pShader,
                           uniformParamaterName,
                           pMusicVisualizerConfig->eqBands[VISUALIZER_BAND_BUFFER - iBand - 1]);
            uniformParamaterName[5] = 0;
        }
        
        pMusicData->changed = false;
    }
    
    hmm_mat4 viewProj = getViewProj(pRenderer);
    shaderSetMat4(pMusicVisualizerConfig->pShader,
                  "viewProj",
                  &viewProj);
    vaBind(pMusicVisualizerConfig->va);
    glCall(glDisable(GL_DEPTH_TEST));
    shaderSetBool(pMusicVisualizerConfig->pShader, "isBorder", true);
    shaderSetFloat(pMusicVisualizerConfig->pShader, "scale", 1.1f);
    drawBufferInstanced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
    shaderSetBool(pMusicVisualizerConfig->pShader, "isBorder", false);
    shaderSetFloat(pMusicVisualizerConfig->pShader, "scale", 1.f);
    if (!isKeyPressed(pRenderer, KEY_UP)) {
        glCall(glEnable(GL_DEPTH_TEST));
    }
    drawBufferInstanced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
}
