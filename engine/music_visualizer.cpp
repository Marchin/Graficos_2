void
initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Material* pMaterial) {
    
    pMusicVisualizerConfig->pMaterial = pMaterial;
    pMusicVisualizerConfig->pBandValues = pMusicData->pFFTMod;
    pMusicVisualizerConfig->bandCount = VISUALIZER_BANDS;
    initVA(&pMusicVisualizerConfig->va);
    vaBind(pMusicVisualizerConfig->va);
    initVB(&pMusicVisualizerConfig->vb, gCubeVertexData, arrayCount(gCubeVertexData) * sizeof(f32));
    
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
    
    materialBindID(pMusicVisualizerConfig->pMaterial->id);
    
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
        shaderSetBool(pMusicVisualizerConfig->pMaterial, "hasFakeTransparency", false);
    } else {
        fillColor(0.f, 0.f, 0.f);
        colorLFN = { 1.f, 0.f, 0.f, 1.f };
        colorHFN = { 1.f, 1.f, 0.f, 1.f };
        colorLFP = { 0.f, 0.f, 1.f, 1.f };
        colorHFP = { 0.f, 1.f, 0.f, 1.f };
        shaderSetBool(pMusicVisualizerConfig->pMaterial, "hasFakeTransparency", true);
    }
    if (blackBorder) {
        borderColor = { 0.f, 0.f, 0.f, 1.f };
    } else {
        borderColor = { 1.f, 1.f, 1.f, 1.f };
    }
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorLFN", &colorLFN); 
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorHFN", &colorHFN);
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorLFP", &colorLFP); 
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorHFP", &colorHFP);
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "borderColor", &borderColor);
    
    shaderSetFloat(pMusicVisualizerConfig->pMaterial, "height", 15.f);
}

void
drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig,
                    MusicData* pMusicData,
                    Renderer* pRenderer) {
    
    materialBindID(pMusicVisualizerConfig->pMaterial->id);
    
    char uniformParamaterName[11] = "band[";
    u32 nameSize = arrayCount(uniformParamaterName);
    u32 bandCount = pMusicVisualizerConfig->bandCount;
    char closedBraket[2] = "]";
    char num[5];
    u32 bands[] = { 1, 16, 24, 32, 40, 48, 64, 96, 128, 160 };
    
    if (pMusicData->changed) {
        u32 count = VISUALIZER_BAND_BUFFER - bandCount;
        for (s32 iBand = count - 1; iBand >= 0; --iBand) {
            pMusicVisualizerConfig->eqBands[iBand + bandCount] = pMusicVisualizerConfig->eqBands[iBand];
        }
        for (u32 iBand = 0; iBand < bandCount; ++iBand) {
            f32 value = pMusicVisualizerConfig->pBandValues[bands[iBand]];
            pMusicVisualizerConfig->eqBands[iBand] = value;
        }
        
        pMusicData->changed = false;
    }
    
    for (u32 iBand = 0; iBand < VISUALIZER_BAND_BUFFER; ++iBand) {
        _itoa(iBand, num, 10);
        strcat(uniformParamaterName, num);
        strcat(uniformParamaterName, closedBraket);
        shaderSetFloat(pMusicVisualizerConfig->pMaterial,
                       uniformParamaterName,
                       pMusicVisualizerConfig->eqBands[VISUALIZER_BAND_BUFFER - iBand - 1]);
        uniformParamaterName[5] = 0;
    }
    
    hmm_mat4 viewProj = getViewProj(pRenderer);
    shaderSetMat4(pMusicVisualizerConfig->pMaterial,
                  "viewProj",
                  &viewProj);
    vaBind(pMusicVisualizerConfig->va);
    glCall(glDisable(GL_DEPTH_TEST));
    shaderSetBool(pMusicVisualizerConfig->pMaterial, "isBorder", true);
    shaderSetFloat(pMusicVisualizerConfig->pMaterial, "scale", 1.1f);
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
    shaderSetBool(pMusicVisualizerConfig->pMaterial, "isBorder", false);
    shaderSetFloat(pMusicVisualizerConfig->pMaterial, "scale", 1.f);
    if (!isKeyPressed(pRenderer, KEY_UP)) {
        glCall(glEnable(GL_DEPTH_TEST));
    }
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
}