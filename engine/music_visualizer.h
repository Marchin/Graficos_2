#ifndef MUSIC_VISUALIZER_H
#define MUSIC_VISUALIZER_H

#define STRIDE 16
#define VISUALIZER_BANDS (256/STRIDE)
#define VISUALIZER_BAND_BUFFER (VISUALIZER_BANDS*8)

const global u32 log2SR = (u32)(log(SAMPLE_RATE)/log(2));

struct FourierData {
    Shader computeShader;
    u32* pReversed;
    f32* pWindow;
    Complex* pTwiddle;
    u32 inputSSBO;
    u32 twiddleSSBO;
};

struct MusicVisualizerConfig {
    FourierData fourierData;
    Shader* pShader;
    f32* pBandValues;
    u32 bandCount;
    u32 va;
    u32 vb;
};

ENGINE_API void setFourierInputAndCalculate(FourierData* pFourierData, f32* pData);
ENGINE_API void initMusicVisualizer(
MusicVisualizerConfig* pMusicVisualizerConfig, MusicData* pMusicData, Shader* pShader);
ENGINE_API void drawMusicVisualizer(
MusicVisualizerConfig* pMusicVisualizerConfig, MusicData* pMusicData, Renderer* pRenderer);
ENGINE_API void setMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig);

global const f32 gCubeVertexData[] = {
    -0.1f,-0.1f,-0.1f, // triangle 1 : begin
    -0.1f,-0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f, // triangle 1 : end
    0.1f, 0.1f,-0.1f, // triangle 2 : begin
    -0.1f,-0.1f,-0.1f,
    -0.1f, 0.1f,-0.1f, // triangle 2 : end
    0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f, 0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    -0.1f,-0.1f,-0.1f,
    -0.1f,-0.1f,-0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f,-0.1f,
    0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f,-0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f,-0.1f, 0.1f,
    0.1f,-0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f, 0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f,-0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f,-0.1f,
    -0.1f, 0.1f,-0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f,-0.1f,
    -0.1f, 0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    0.1f,-0.1f, 0.1f
};
#endif //MUSIC_VISUALIZER_H