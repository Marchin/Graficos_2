#ifndef PHYSICS_H
#define PHYSICS_H
#define COLLIDERS_AMOUNT 512

struct ENGINE_API BoxCollider {
	f32 halfWidth;
	f32 halfHeight;
	f32 mass;
	b32 isStatic;
	b32 isTrigger;
	b32 isActive;
	Transform* pTransform;
};

struct ENGINE_API CollisionManager {
	BoxCollider colliders[COLLIDERS_AMOUNT];
	u32 count;
};

ENGINE_API void initBoxCollider(BoxCollider* pBC);
ENGINE_API void checkCollision(CollisionManager* pCM, i32 index1, i32 index2);
ENGINE_API void updateCollision(CollisionManager* pCM);
ENGINE_API BoxCollider* getNewBoxCollider(CollisionManager* pCM);
ENGINE_API inline void removeBoxCollider(CollisionManager* pCM, BoxCollider* pCollider);
#endif //PHYSICS_H