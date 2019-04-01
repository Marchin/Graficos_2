internal void
initCharacter(Character* pCharacter, CollisionManager* pCM) {
	Shader materialSprite;
    initShader(&materialSprite, "..//resources//shaders//vTexture.glsl", 
               "..//resources//shaders//fTexture.glsl");
    
    char* pSSPath = "..//resources//spriteSheet.png";
	f32 squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f,  0.5f, 0.f
	};
	u32 frames[] = { 0, 2, 5 };
    initSpriteSheet(&pCharacter->spriteSheet, &materialSprite, 
                    pSSPath, squareVertices);
	spriteSheetSetFrameSize(&pCharacter->spriteSheet, 64);
	BoxCollider* boxCollider = getNewBoxCollider(pCM);
	boxCollider->halfHeight = 0.5f;
	boxCollider->halfWidth = 0.5f;
	boxCollider->pTransform = &pCharacter->spriteSheet.sprite.transform;
	boxCollider->isStatic = false;
	boxCollider->isTrigger = false;
    pCharacter->pCollider = boxCollider;
    initAnimation(&pCharacter->animation, &pCharacter->spriteSheet, 
                  frames, ArrayCount(frames));
}

inline void
freeCharacter(Character* pCharacter, CollisionManager* pCM) {
	removeBoxCollider(pCM, pCharacter->pCollider);
}

inline void
moveCharacter(Character* pCharacter, f32 x, f32 y) {
	if (!pCharacter->pCollider->isStatic) {
		transformTranslate(&pCharacter->spriteSheet.sprite.transform, x, y, 0.f);
	}
}

internal void
drawCharacter(Character* pCharacter, Renderer* pRenderer) {
	drawSprite(&pCharacter->spriteSheet.sprite, pRenderer);
}

inline void
updateCharacter(Character* pCharacter, f32 deltaTime) {
	updateAnimation(&pCharacter->animation, deltaTime);
}
