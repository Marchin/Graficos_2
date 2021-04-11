#ifndef GAME_H
#define GAME_H

struct Game {
    Camera camera;
    Transform empty;
    WAV sound;
    MusicVisualizerConfig musicVisualizerConfig;
    Character character;
    Character character2;
    Character character3;
    Character character4;
    Shader musicVisualizerShader;
    Shader modelShader;
    MusicData musicData;
    ModelData modelData;
    f32 camX, camY;
};

#endif //GAME_H