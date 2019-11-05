#ifndef GAME_H
#define GAME_H

struct Game {
    Camera camera;
    Transform empty;
    WAV sound;
    MusicVisualizerConfig musicVisualizerConfig;
    Material musicVisualizerMaterial;
    f32 camX, camY;
};

#endif //GAME_H