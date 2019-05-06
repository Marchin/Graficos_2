#ifndef GAME_H
#define GAME_H

struct Game {
    Triangle triangle;
    ColorSquare cs;
    Circle circle;
    Animation anim;
    SpriteSheet ss;
    Character character1, character2, character3;
    SpriteSheet tileset;
    Tilemap tilemap;
    Camera camera;
    Model model;
    f32 camX, camY;
    f32 timer;
};

#endif //GAME_H