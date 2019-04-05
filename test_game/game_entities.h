#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

struct Character {
	BoxCollider* pCollider;
    SpriteSheet spriteSheet;
	Animation animation;
    Transform transform;
    f32 speed;
};

#endif //GAME_ENTITIES_H