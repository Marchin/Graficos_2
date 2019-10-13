#ifndef GAME_H
#define GAME_H


struct Game {
    Camera camera;
    Character floor;
    Character cubeA;
    Character cubeB;
    Character cubeC;
    Character cubeD;
    Character cubeE;
    Character wall1;
    Character wall2;
    Character wall3;
    Transform scene;
    Level level;
    f32 camX, camY;
    f32 timer;
};

#endif //GAME_H