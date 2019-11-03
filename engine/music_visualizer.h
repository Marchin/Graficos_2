#ifndef MUSIC_VISUALIZER_H
#define MUSIC_VISUALIZER_H

#define VISUALIZER_BANDS 8
#define VISUALIZER_BAND_BUFFER VISUALIZER_BANDS*7

struct MusicVisualizerConfig {
    Material* pMaterial;
    f32* pBandValues;
    u32 bandCount;
    u32 bandsToSkip;
    u32 va;
    u32 vb;
    f32 eqBands[VISUALIZER_BAND_BUFFER];
    u32 counter;
};

ENGINE_API void initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Material* pMaterial);
ENGINE_API void drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Renderer* pRenderer);
ENGINE_API void setMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig);
#endif //MUSIC_VISUALIZER_H