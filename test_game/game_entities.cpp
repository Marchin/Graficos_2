////////////////////////////////

//Figure

////////////////////////////////

inline void
moveFigure(Figure* pFigure, f32 x, f32 y) {
	if (!pFigure->pCollider->isStatic) {
		transformTranslate(&pFigure->transform, x, y, 0.f);
	}
}

internal b32
drawFigure(void* pFigure, Renderer* pRenderer) {
    Figure* pCastedFigure = (Figure*)pFigure;
	drawSpriteRenderer(&pCastedFigure->spriteSheet.spriteRenderer, pRenderer);
    
    return true;
}

inline void
updateFigure(void* pFigure, f32 deltaTime) {
    Figure* pCastedFigure = (Figure*)pFigure;
	updateAnimation(&pCastedFigure->animation, deltaTime);
}

internal void
initFigure(Figure* pFigure, CollisionManager* pCM) {
	initTransform(&pFigure->transform);
    initShader(&pFigure->shader, "Texture", "..//resources//shaders//vTexture.glsl", 
               "..//resources//shaders//fTexture.glsl");
    
    char* pSSPath = "..//resources//spriteSheet.png";
	f32 squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
        0.5f, -0.5f, 0.f,
        0.5f,  0.5f, 0.f
	};
	u32 frames[] = { 0, 2, 5 };
    initSpriteSheet(&pFigure->spriteSheet, 
                    &pFigure->transform, &pFigure->shader, 
                    pSSPath, squareVertices);
	spriteSheetSetFrameSize(&pFigure->spriteSheet, 64);
	BoxCollider* boxCollider = getNewBoxCollider(pCM);
	boxCollider->halfHeight = 0.5f;
	boxCollider->halfWidth = 0.5f;
	boxCollider->pTransform = &pFigure->transform;
	boxCollider->isStatic = false;
	boxCollider->isTrigger = false;
    pFigure->pCollider = boxCollider;
    initAnimation(&pFigure->animation, &pFigure->spriteSheet, 
                  frames, arrayCount(frames));
    
    pFigure->transform.pEntity = pFigure;
    pFigure->transform.draw = drawFigure;
    pFigure->transform.update = updateFigure;
}

inline void
freeFigure(Figure* pFigure, CollisionManager* pCM) {
	removeBoxCollider(pCM, pFigure->pCollider);
    freeSpriteSheet(&pFigure->spriteSheet);
}


////////////////////////////////

//Character

////////////////////////////////

internal void
drawCharacter(void* pCharacter, Renderer* pRenderer, hmm_mat4 model) {
    Character* pCastedCharacter = (Character*)pCharacter;
	drawModel(&pCastedCharacter->model, pRenderer);
}

inline void
updateCharacter(void* pCharacter, f32 deltaTime) {
    //Character* pCastedCharacter = (Character*)pCharacter;
}

internal void
initCharacter(Character* pCharacter, const char* pModelPath) {
    initTransform(&pCharacter->transform);
    strcpy(pCharacter->transform.name, pModelPath);
    pCharacter->transform.pEntity = pCharacter;
    //pCharacter->transform.draw = drawCharacter;
    //pCharacter->transform.update = updateCharacter;
    
    initShader(&pCharacter->shader, "Model", "..//resources//shaders//vModel.glsl", 
               "..//resources//shaders//fModel.glsl");
    
    initModel(&pCharacter->model, pModelPath, &pCharacter->shader, &pCharacter->transform);
}

internal void
freeCharacter(Character* pCharacter) {
    freeModel(&pCharacter->model);
}