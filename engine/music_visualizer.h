#ifndef MUSIC_VISUALIZER_H
#define MUSIC_VISUALIZER_H

#define STRIDE 16
#define VISUALIZER_BANDS (256/STRIDE)
#define TIME_SIZE 8
#define VISUALIZER_BAND_BUFFER (VISUALIZER_BANDS*TIME_SIZE)

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
    u32 bandAmount;
    u32 stride;
    u32 timeSize;
    u32 va;
};

ENGINE_API void setFourierInputAndCalculate(FourierData* pFourierData, f32* pData);
ENGINE_API void initMusicVisualizer(
MusicVisualizerConfig* pMusicVisualizerConfig, MusicData* pMusicData, Shader* pShader);
ENGINE_API void drawMusicVisualizer(
MusicVisualizerConfig* pMusicVisualizerConfig, MusicData* pMusicData, Renderer* pRenderer);
ENGINE_API void setMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig);

#endif //MUSIC_VISUALIZER_H