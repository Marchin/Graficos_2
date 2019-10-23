#ifndef GAME_H
#define GAME_H


struct Game {
    Camera camera;
    Character scene;
    Character cubeE;
    WAV sound;
    Level level;
    f32 camX, camY;
    f32 timer;
};

#endif //GAME_H