#ifndef GAME_H
#define GAME_H


struct Game {
    Triangle triangle;
    ColorSquare cs;
    Circle circle;
    Animation anim;
    SpriteSheet ss;
    Figure figure1, figure2, figure3;
    SpriteSheet tileset;
    Tilemap tilemap;
    Camera camera;
    Model model;
    Model model2;
    Transform scene;
    f32 camX, camY;
    f32 timer;
};

#endif //GAME_H