internal void
initGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    pGame->camX = {};
    pGame->camY = {};
    
    Shader basicMaterial;
    initShader(&basicMaterial, "..//resources//shaders//vShader.glsl", 
               "..//resources//shaders//fShader.glsl");
    
    Shader colorMaterial;
    initShader(&colorMaterial, "..//resources//shaders//vColor.glsl", 
               "..//resources//shaders//fColor.glsl");
    
    Shader textureMaterial;
    initShader(&textureMaterial, "..//resources//shaders//vTexture.glsl", 
               "..//resources//shaders//fTexture.glsl");
    
    char* pSpritePath = "..//resources//container.jpg";
    char* pSpriteSheetPath = "..//resources//spriteSheet.png";
    
    float vertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		0.5f,  0.f, 0.f
	};
    float squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f,  0.5f, 0.f
	};
	float squareColors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 1.f, 0.f
	};
	float squareUV[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f
	};
    u32 frames[] = {
        0, 7, 2, 5, 4,
    };
    
#if 0
    initTriangle(&pGame->triangle, &basicMaterial, &vertices, sizeof(vertices));
    transformScale(&pGame->triangle.transform, 9.f, 3.f, 3.f);
    
    initColorSquare(&pGame->cs, &colorMaterial, & squareVertices, squareColors);
    transformScale(&pGame->cs.transform, 5.f, 5.f, 5.f);
    
    initCircle(&pGame->circle, &basicMaterial, 16, 5.f);
    transformTranslate(&pGame->circle.transform, 6.f, 2.f, 0.f);
    
    initSprite(&pGame->sprite, &textureMaterial, pSpritePath, squareVertices, squareUV);
    transformTranslate(&pGame->sprite.transform, -6.f, 0.f, 0.f);
    transformScale(&pGame->sprite.transform, 6.f, 6.f, 6.f);
    
    initAnimation(&pGame->anim, &ss, frames, ArrayCount(frames));
#endif
    
    initSpriteSheet(&pGame->ss, &textureMaterial, pSpriteSheetPath, 
                    squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->ss, 64);
    
    initCharacter(&pGame->character1, pCM);
    initCharacter(&pGame->character2, pCM);
    initCharacter(&pGame->character3, pCM);
    moveCharacter(&pGame->character1, 20.f, -9.f);
    moveCharacter(&pGame->character2, 9.f, 0.f);
    moveCharacter(&pGame->character3, 0.f, -7.f);
    pGame->character2.pCollider->mass = 5.f;
    
	char* pTilesetPath = "..//resources//tileset.png";
	initSpriteSheet(&pGame->tileset, &textureMaterial, pTilesetPath, squareVertices, squareUV);
    spriteSheetSetFrameSize(&pGame->tileset, 32);
    
    initTilemap(&pGame->tilemap, "..//resources//tilemap.csv", 
                &pGame->tileset, &textureMaterial, pRenderer);
	i32 collisionableTiles[] = { 37, 45, 46, 47, 55 };
	tilemapSetCollisionableTiles(&pGame->tilemap, 
                                 collisionableTiles, 
                                 sizeof(collisionableTiles) / sizeof(i32));
    
	tilemapRegisterColliders(&pGame->tilemap, pGame->character1.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->character2.pCollider);
	tilemapRegisterColliders(&pGame->tilemap, pGame->character3.pCollider);
}

internal void
updateGame(Game* pGame, Renderer* pRenderer, Time* pTime, CollisionManager* pCM = 0) {
    //transformTranslate(&pGame->sprite.transform, .01f, 0.f, 0.f);
    //transformTranslate(&pGame->cs.transform, -.01f, 0f, 0.f);
    //drawTriangle(&pGame->triangle, pRenderer);
    //drawColorSquare(&pGame->cs, pRenderer);
    //drawCircle(&pGame->circle, pRenderer);
    //drawSprite(&pGame->sprite, pRenderer);
    //drawSprite(&pGame->ss.sprite, pRenderer);
    //updateAnimation(&pGame->anim, deltaTime);
    //transformRotateY(&pGame->cs.transform, 5.f);
    //transformRotateZ(&pGame->sprite.transform, 5.f);
    
    if (pGame->camX > 32.f) {
        pGame->camX = 0.f;
        pGame->camY -= 5.f;
        if (pGame->camY < -35.f) {
            pGame->camY = 0.f;
        }
    } else {
        pGame->camX += 2.f  * pTime->deltaTime;
    }
    setCameraPosition(pRenderer, pGame->camX, pGame->camY);
    drawTilemap(&pGame->tilemap, pRenderer);
    moveCharacter(&pGame->character1, -2.f * pTime->deltaTime, 0.f);
    updateCharacter(&pGame->character1, pTime->deltaTime);
    drawCharacter(&pGame->character1, pRenderer);
    moveCharacter(&pGame->character2, 0.f, -2.f * pTime->deltaTime);
    updateCharacter(&pGame->character2, pTime->deltaTime);
    drawCharacter(&pGame->character2, pRenderer);
    moveCharacter(&pGame->character3, 2.f * pTime->deltaTime, 0.f);
    updateCharacter(&pGame->character3, pTime->deltaTime);
    drawCharacter(&pGame->character3, pRenderer);
    tilemapCheckCollisions(&pGame->tilemap);
}