global const f32 gCubeVertexData[] = {
    -0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};

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
    pMusicVisualizerConfig->counter = 0;
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
    shaderSetInt(pMusicVisualizerConfig->pMaterial, "isWhite", 1);
    shaderSetFloat(pMusicVisualizerConfig->pMaterial, "scale", 1.1f);
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
    shaderSetInt(pMusicVisualizerConfig->pMaterial, "isWhite", 0);
    shaderSetFloat(pMusicVisualizerConfig->pMaterial, "scale", 1.f);
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, VISUALIZER_BAND_BUFFER);
    glCall(glEnable(GL_DEPTH_TEST));
}