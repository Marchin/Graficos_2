void
initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Material* pMaterial) {
    pMusicVisualizerConfig->pMaterial = pMaterial;
    pMusicVisualizerConfig->pBandValues = getFFTModResult();
    pMusicVisualizerConfig->bandCount = 8;
    pMusicVisualizerConfig->bandsToSkip = (getFFTModSize()/2)/pMusicVisualizerConfig->bandCount;
    f32 squareVertices[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    initVA(&pMusicVisualizerConfig->va);
    vaBind(pMusicVisualizerConfig->va);
    initVB(&pMusicVisualizerConfig->vb, squareVertices, 18 * sizeof(f32));
    
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
    u32 bandIndex = FRAMES_PER_BUFFER/2;
    for (u32 iBand = 0; iBand < bandCount; ++iBand) {
        _itoa(iBand, num, 10);
        strcat(uniformParamaterName, num);
        strcat(uniformParamaterName, closedBraket);
        shaderSetFloat(pMusicVisualizerConfig->pMaterial,
                       uniformParamaterName,
                       pMusicVisualizerConfig->pBandValues[bandIndex]);
        uniformParamaterName[5] = 0;
        bandIndex += pMusicVisualizerConfig->bandsToSkip;
    }
    hmm_mat4 viewProj = getViewProj(pRenderer);
    shaderSetMat4(pMusicVisualizerConfig->pMaterial,
                  "viewProj",
                  &viewProj);
    vaBind(pMusicVisualizerConfig->va);
    drawBufferInstenced(0, 6, bandCount);
    printf("HEY");
}