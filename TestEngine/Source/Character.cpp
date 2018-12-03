#include "../Headers/Character.h"

Character::Character(Renderer* pRenderer) {
	Shader sSprite("Resources/Shaders/ShaderTexture/vTexture.glsl",
		"Resources/Shaders/ShaderTexture/fTexture.glsl");
	Material materialSprite(sSprite);

	const char* spriteSheetPath = "Resources/spriteSheet2.png";
	float squareVertices[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f,  0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.5f,  0.5f, 0.f
	};
	float squareUV[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f,
		1.f, 1.f
	};
	unsigned int frames[] = { 0, 2, 5 };
	m_pSpriteSheet = new SpriteSheet(pRenderer, materialSprite, spriteSheetPath, squareVertices, squareUV);
	m_pSpriteSheet->SetFrameSize(64);
	m_pBoxCollider = CollisionManager::GetInstance()->Register();
	m_pBoxCollider->halfHeight = 0.5f;
	m_pBoxCollider->halfWidth = 0.5f;
	m_pBoxCollider->pEntity = m_pSpriteSheet;
	m_pBoxCollider->isStatic = false;
	m_pBoxCollider->isTrigger = false;
	m_pAnimation = new Animation(m_pSpriteSheet, frames, sizeof(frames) / (sizeof(unsigned int)));
}

Character::~Character() {
	CollisionManager::GetInstance()->Remove(m_pBoxCollider);
	delete m_pAnimation;
	delete m_pSpriteSheet;
}

void Character::Move(float x, float y) {
	if (!m_pBoxCollider->isStatic) {
		m_pSpriteSheet->Translate(x, y, 0.f);
		m_pSpriteSheet->GetPosition(
			m_pBoxCollider->position.x,
			m_pBoxCollider->position.y,
			m_pBoxCollider->position.z);
	}
}

void Character::Draw() {
	m_pSpriteSheet->Draw();
	m_pSpriteSheet->GetPosition(
		m_pBoxCollider->position.x,
		m_pBoxCollider->position.y,
		m_pBoxCollider->position.z);
}

void Character::SetStatic(bool state){
	m_pBoxCollider->isStatic = true;
}

void Character::Update(float deltaTime) {
	m_pAnimation->Update(deltaTime);
}

void Character::SetMass(float mass) {
	m_pBoxCollider->mass = mass;
}

BoxCollider* Character::GetCollider() {
	return m_pBoxCollider;
}
