#ifndef GAME_ENTITIES_H
#define GAME_ENTITIES_H

struct Character {
	BoxCollider* pCollider;
    SpriteSheet spriteSheet;
	Animation animation;
    float speed;
};

#endif //GAME_ENTITIES_H