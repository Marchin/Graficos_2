#ifndef MUSIC_VISUALIZER_H
#define MUSIC_VISUALIZER_H

struct MusicVisualizerConfig {
    Material* pMaterial;
    f32* pBandValues;
    u32 bandCount;
    u32 bandsToSkip;
    u32 va;
    u32 vb;
};

ENGINE_API void initMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Material* pMaterial);
ENGINE_API void drawMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig, Renderer* pRenderer);
ENGINE_API void setMusicVisualizer(MusicVisualizerConfig* pMusicVisualizerConfig);
#endif //MUSIC_VISUALIZER_H