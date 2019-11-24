#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

struct BasicGO {
    Transform transform;
    Shader shader;
};

struct Figure {
	BoxCollider* pCollider;
    SpriteSheet spriteSheet;
	Animation animation;
    Transform transform;
    Shader shader;
    f32 speed;
};

struct Character {
    Model model;
    Transform transform;
    Shader shader;
};

#endif //GAME_ENTITIES_H