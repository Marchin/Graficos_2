#ifndef GAME_H
#define GAME_H

struct Game {
    Triangle triangle;
    ColorSquare cs;
    Circle circle;
    Sprite sprite;
    Animation anim;
    SpriteSheet ss;
    Character character1, character2, character3;
    SpriteSheet tileset;
    Tilemap tilemap;
    f32 camX, camY;
    
};

#endif //GAME_H