ENGINE_API void
initBoxCollider(BoxCollider* pBC) {
    *pBC = {};
    pBC->mass = 1.f;
    pBC->isActive = true;
}

ENGINE_API void
checkCollision(CollisionManager* pCM, s32 index1, s32 index2) {
	BoxCollider* pCollider1 = &pCM->colliders[index1];
	BoxCollider* pCollider2 = &pCM->colliders[index2];
	f32 deltaX = pCollider1->pTransform->position.x - pCollider2->pTransform->position.x;
	f32 deltaY = pCollider1->pTransform->position.y - pCollider2->pTransform->position.y;
    f32 halfWidth1 = pCollider1->halfWidth * pCollider1->pTransform->scale.x;
    f32 halfHeight1 = pCollider1->halfHeight * pCollider1->pTransform->scale.y;
    f32 halfWidth2 = pCollider2->halfWidth * pCollider2->pTransform->scale.x;
    f32 halfHeight2 = pCollider2->halfHeight * pCollider2->pTransform->scale.y;
    
	if (HMM_ABS(deltaX) < halfWidth1 + halfWidth2 &&
		HMM_ABS(deltaY) < halfHeight1 + halfHeight2) {
        
		f32 pX = halfWidth1 + halfWidth2 - HMM_ABS(deltaX);
		f32 pY = halfHeight1 + halfHeight2 - HMM_ABS(deltaY);
		f32 totalMass = pCollider1->mass + pCollider2->mass;
		if (pX < pY) {
			if (!pCollider1->isStatic && !pCollider1->isTrigger) {
				transformTranslate(
                    pCollider1->pTransform,
                    pX * (deltaX > 0.f ? 1.f : -1.f) * (pCollider2->mass / totalMass),
                    0.f,
                    0.f);
			}
			if (!pCollider2->isStatic && !pCollider2->isTrigger) {
                transformTranslate(
                    pCollider2->pTransform,
					pX * (deltaX > 0.f ? -1.f : 1.f) * (pCollider1->mass / totalMass),
					0.f,
					0.f);
			}
		} else {
			if (!pCollider1->isStatic && !pCollider1->isTrigger) {
                transformTranslate(
                    pCollider1->pTransform,
					0.f,
					pY * (deltaY > 0.f ? 1.f : -1.f) * (pCollider2->mass / totalMass),
					0.f);
			}
			if (!pCollider2->isStatic && !pCollider2->isTrigger) {
                transformTranslate(
                    pCollider2->pTransform,
                    0.f,
					pY * (deltaY > 0.f ? -1.f : 1.f) * (pCollider1->mass / totalMass),
					0.f);
			}
		}
	}
}

ENGINE_API void
updateCollision(CollisionManager* pCM) {
	u8 counter = 0;
	u8 counter2;
	if (pCM->count < 2) { //if there aren't at least 2 colliders to check
		return;
	}
	for (s32 i = 0; counter < pCM->count - 1; ++i) {
		if (!pCM->colliders[i].isActive) { continue; }
		++counter;
		counter2 = counter;
		for (s32 j = counter2; counter2 < pCM->count; ++j) {
			if (!pCM->colliders[j].isActive) { continue; }
			counter2++;
			if ((!pCM->colliders[i].isStatic && !pCM->colliders[i].isTrigger) ||
				(!pCM->colliders[j].isStatic && !pCM->colliders[j].isTrigger)) {
                
				checkCollision(pCM, i, j);
			}
		}
	}
}

ENGINE_API BoxCollider* 
getNewBoxCollider(CollisionManager* pCM) {
	int id;
	for (id = 0; id < COLLIDERS_AMOUNT; ++id) {
		if (!pCM->colliders[id].isActive) {
			pCM->count++;
			break;
		}
	}
    if (id < COLLIDERS_AMOUNT) {
        BoxCollider* pNewBC = &pCM->colliders[id];
        initBoxCollider(pNewBC);
        return pNewBC;
    } else {
        return NULL;
    }
}

ENGINE_API inline void
removeBoxCollider(CollisionManager* pCM, BoxCollider* pCollider) {
	pCollider->isActive = false;
	--pCM->count;
}
