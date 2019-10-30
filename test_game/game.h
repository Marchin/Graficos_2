#ifndef GAME_H
#define GAME_H

struct Game {
    Camera camera;
    Character scene;
    Character cubeE;
    Transform empty;
    WAV sound;
    Level level;
    MusicVisualizerConfig musicVisualizerConfig;
    Material musicVisualizerMaterial;
    f32 camX, camY;
    f32 timer;
};

#endif //GAME_H