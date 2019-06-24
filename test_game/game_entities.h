#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

struct BasicGO {
    Transform transform;
    Material material;
};

struct Figure {
	BoxCollider* pCollider;
    SpriteSheet spriteSheet;
	Animation animation;
    Transform transform;
    Material material;
    f32 speed;
};

struct Character {
    Model model;
    Transform transform;
    Material material;
};

#endif //GAME_ENTITIES_H