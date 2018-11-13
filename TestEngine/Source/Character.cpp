#include "../Headers/Character.h"

Character::Character(Renderer* pRenderer){
	Shader sSprite("Resources/Shaders/ShaderTexture/vTexture.glsl",
		"Resources/Shaders/ShaderTexture/fTexture.glsl");
	Material materialSprite(sSprite);

	const char* spriteSheetPath = "Resources/spriteSheet2.png";
	m_pSpriteSheet = new SpriteSheet(pRenderer, materialSprite, spriteSheetPath);
	m_pBoxCollider = CollisionManager::GetInstance().Register();
	m_pBoxCollider->halfHeight = 3.f;
	m_pBoxCollider->halfWidth = 3.f;
	m_pBoxCollider->pEntity = m_pSpriteSheet;
	m_pBoxCollider->isStatic = false;
	m_pBoxCollider->isTrigger = false;
}

Character::~Character() {
	CollisionManager::GetInstance().Remove(m_pBoxCollider);
	delete m_pSpriteSheet;
}
