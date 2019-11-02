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
}

void
drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Renderer* pRenderer) {
    materialBindID(pMusicVisualizerConfig->pMaterial->id);
    
    char uniformParamaterName[11] = "band[";
    u32 nameSize = arrayCount(uniformParamaterName);
    
    u32 bandCount = pMusicVisualizerConfig->bandCount;
    char closedBraket[2] = "]";
    char num[5];
    u32 bandIndex = 1;
    u32 bandsToSkip = pMusicVisualizerConfig->bandsToSkip;
    u32 bands[VISUALIZER_BANDS + 1] = { 1, 16, 32, 64, 96, 128, 160, 192 };
    //u32 bands[VISUALIZER_BANDS + 1] = { 1, 64, 128, 192, 256, 512, 768 , 1024};
    u32 halfBands = VISUALIZER_BANDS/2;
    u32 lastPos = bands[0];
    for (u32 iBand = 0; iBand < bandCount; ++iBand) {
        _itoa(iBand, num, 10);
        strcat(uniformParamaterName, num);
        strcat(uniformParamaterName, closedBraket);
#if 0
        f32 accum = {};
        u32 newPos;
        newPos = bands[iBand + 1];
#if 0
        if (iBand < bandCount - 1) {
            newPos = bands[iBand + 1];
        } else {
            newPos = bands[iBand];
        }
#endif
        u32 diff = (newPos - lastPos)/2;
        for (u32 i = lastPos; i <= diff; ++i) {
            accum += (pMusicVisualizerConfig->pBandValues[i]/diff);
        }
        lastPos += diff;
#endif
        f32 value = pMusicVisualizerConfig->pBandValues[bands[iBand]];
        if (iBand < halfBands) { value *= 0.25f; }
        shaderSetFloat(pMusicVisualizerConfig->pMaterial,
                       uniformParamaterName,
                       value);
        //pMusicVisualizerConfig->pBandValues[bands[iBand]]);
        //pMusicVisualizerConfig->pBandValues[bandIndex]);
        //pMusicVisualizerConfig->pBandValues[0]));
        //accum);
        uniformParamaterName[5] = 0;
        bandIndex += pMusicVisualizerConfig->bandsToSkip;
    }
    hmm_mat4 viewProj = getViewProj(pRenderer);
    shaderSetMat4(pMusicVisualizerConfig->pMaterial,
                  "viewProj",
                  &viewProj);
    vaBind(pMusicVisualizerConfig->va);
    drawBufferInstenced(0, arrayCount(gCubeVertexData)/3, bandCount);
}