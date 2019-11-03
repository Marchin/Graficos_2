void
initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Material* pMaterial) {
    pMusicVisualizerConfig->pMaterial = pMaterial;
    pMusicVisualizerConfig->pBandValues = getFFTModResult();
    pMusicVisualizerConfig->bandCount = VISUALIZER_BANDS;
    //skip less low, more high freq
    pMusicVisualizerConfig->bandsToSkip = (getFFTModSize()/2)/pMusicVisualizerConfig->bandCount;
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
    hmm_vec4 colorLFN = { 1.f, 0.f, 0.f, 1.f };
    hmm_vec4 colorHFN = { 1.f, 1.f, 0.f, 1.f };
    hmm_vec4 colorLFP = { 0.f, 0.f, 1.f, 1.f };
    hmm_vec4 colorHFP = { 0.f, 1.f, 1.f, 1.f };
    hmm_vec4 borderColor = { 1.f, 1.f, 1.f, 1.f };
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorLFN", &colorLFN); 
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorHFN", &colorHFN);
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorLFP", &colorLFP); 
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "colorHFP", &colorHFP);
    shaderSetVec4(pMusicVisualizerConfig->pMaterial, "borderColor", &borderColor); 
}

void
drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Renderer* pRenderer) {
    materialBindID(pMusicVisualizerConfig->pMaterial->id);
    
    char uniformParamaterName[11] = "band[";
    u32 nameSize = arrayCount(uniformParamaterName);
    u32 bandCount = pMusicVisualizerConfig->bandCount;
    char closedBraket[2] = "]";
    char num[5];
    u32 bands[VISUALIZER_BANDS + 1] = { 1, 16, 32, 64, 96, 128, 160, 192 };
    u32 halfBands = VISUALIZER_BANDS/2;
    
    if (hasMusicBufferChanged()) {
        u32 count = VISUALIZER_BAND_BUFFER - bandCount;
        for (s32 iBand = count - 1; iBand >= 0; --iBand) {
            pMusicVisualizerConfig->eqBands[iBand + bandCount] = pMusicVisualizerConfig->eqBands[iBand];
        }
        for (u32 iBand = 0; iBand < bandCount; ++iBand) {
            f32 value = pMusicVisualizerConfig->pBandValues[bands[iBand]];
            if (iBand < halfBands) { value *= 0.25f; }
            pMusicVisualizerConfig->eqBands[iBand] = value;
        }
        
        clearMusicBufferChanged();
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
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
    glCall(glEnable(GL_DEPTH_TEST));
}