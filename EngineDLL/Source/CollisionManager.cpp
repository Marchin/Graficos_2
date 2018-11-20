#include "../Headers/CollisionManager.h"
#include "../Headers/Entity.h"
#include <algorithm>

CollisionManager::~CollisionManager() {
}

CollisionManager* CollisionManager::GetInstance() {
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::Update() {
	for (size_t i = 0; i < m_count - 1; i++){
		for (size_t j = 1; j < m_count; j++) {
			if ((!m_colliders[i].isStatic && !m_colliders[i].isTrigger) ||
				(!m_colliders[j].isStatic && !m_colliders[j].isTrigger)) {

				CheckCollision(i, j);
			}
		}
	}
}

BoxCollider* CollisionManager::Register() {
	return &m_colliders[m_count++];
}

void CollisionManager::Remove(BoxCollider * collider) {
	//m_colliders.erase(std::find(m_colliders.begin(), m_colliders.end(), *collider));
}

void CollisionManager::CheckCollision(int index1, int index2){
	BoxCollider* collider1 = &m_colliders[index1];
	BoxCollider* collider2 = &m_colliders[index2];
	float deltaX = collider1->position.x - collider2->position.x;
	float deltaY = collider1->position.y - collider2->position.y;

	if (glm::abs(deltaX) < collider1->halfWidth + collider2->halfWidth &&
		glm::abs(deltaY) < collider1->halfHeight + collider2->halfHeight) {

		float pX = collider1->halfWidth + collider2->halfWidth - glm::abs(deltaX);
		float pY = collider1->halfHeight + collider2->halfHeight - glm::abs(deltaY);
		if (pX < pY) {
			if (!collider1->isStatic && !collider1->isTrigger) {
				collider1->pEntity->Translate(
					pX * (deltaX > 0.f? 1.f : -1.f),
					0.f,
					0.f
				);
			}
			if (!collider2->isStatic && !collider2->isTrigger) {
				collider2->pEntity->Translate(
					pX * (deltaX > 0.f ? -1.f : 1.f),
					0.f,
					0.f
				);
			}
		} else {
			if (!collider1->isStatic && !collider1->isTrigger) {
				collider1->pEntity->Translate(
					0.f,
					pY * (deltaY > 0.f ? 1.f : -1.f),
					0.f
				);
			}
			if (!collider2->isStatic && !collider2->isTrigger) {
				collider2->pEntity->Translate(
					0.f,
					pY * (deltaY > 0.f ? -1.f : 1.f),
					0.f
				);
			}
		}
	}
}

CollisionManager::CollisionManager() : m_count(0){
}
