#ifndef GAME_H
#define GAME_H

struct Game {
    Camera camera;
    Transform empty;
    WAV sound;
    MusicVisualizerConfig musicVisualizerConfig;
    Shader musicVisualizerShader;
    MusicData musicData;
    f32 camX, camY;
};

#endif //GAME_H