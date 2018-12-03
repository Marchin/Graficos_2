#include "../Headers/Tilemap.h"
#include "../Headers/TilemapLoader.h"
#include "../Headers/VertexBufferLayout.h"

Tilemap::Tilemap(const char* fileDir, SpriteSheet* pSpriteSheet,
	Renderer* pRenderer, Material material)
	: m_spriteSheet(*pSpriteSheet), m_pRenderer(pRenderer), m_material(material),
	m_width(0), m_height(0), m_tileSize(1.f), m_collidersCount(0) {

	TilemapLoader loader(fileDir, &m_pIDs, &m_width, &m_height);
	m_visibleWidth = m_pRenderer->GetCameraWidth() / m_tileSize + 2; //one extra column at each side
	m_visibleHeight = m_pRenderer->GetCameraHeight() / m_tileSize + 2; //one extra row at each side
	m_pVisibleTiles = new int[m_visibleWidth * 	m_visibleHeight];
	m_pRenderer->GetCameraPosition(&m_cacheCamPosX, &m_cacheCamPosY);

	//Genero los strides para el VertexArray
	VertexBufferLayout layoutPosition, layoutUV;
	layoutPosition.Push<float>(3);
	layoutUV.Push<float>(2);
	m_material.Bind();
	m_spriteSheet.GetTexture()->Bind(0);
	material.SetIntProperty("tex",0);

	CalculateVertexPosition();
	CalculateUV();
	CalculateVisibleTiles();

	m_va.AddBufferByLocation(m_vbPosition, layoutPosition, 0);
	m_va.AddBufferByLocation(m_vbUV, layoutUV, 1);
}

Tilemap::~Tilemap() {
	if (m_pIDs != NULL) {
		delete[] m_pIDs;
	}
	if (m_pUVData != NULL) {
		delete[] m_pUVData;
	}
	if (m_pVisibleTiles != NULL) {
		delete[] m_pVisibleTiles;
	}
	if (m_pCoords != NULL) {
		delete[] m_pCoords;
	}
}

void Tilemap::Draw() {
	float camPosX, camPosY;
	m_pRenderer->GetCameraPosition(&camPosX, &camPosY);
	if ((glm::abs((m_cacheCamPosX - camPosX)) >= 1.f) ||
		(glm::abs((m_cacheCamPosY - camPosY) >= 1.f))){

		m_cacheCamPosX = camPosX;
		m_cacheCamPosY = camPosY;
		CalculateVisibleTiles();
	}
	m_material.Bind();
	m_spriteSheet.GetTexture()->Bind(0);
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	for (int i = 0; i < m_visibleHeight; i++) {
		for (int j = 0; j < m_visibleWidth; j++) {
			if (m_pVisibleTiles[(j + (m_visibleWidth*i))] >= 0) {
				m_pRenderer->DrawBufferStrip(m_pVisibleTiles[(j + (m_visibleWidth*i))] * 4, 4);
			}
		}
	}
}

void Tilemap::CheckCollisions() {
	for (unsigned int i = 0; i < m_collidersCount;) {
		if (m_colliders[i] != NULL) {
			CheckSideColliding(m_colliders[i], m_colliders[i]->halfWidth, 0.f);
			CheckSideColliding(m_colliders[i], -m_colliders[i]->halfWidth, 0.f);
			CheckSideColliding(m_colliders[i], 0.f, m_colliders[i]->halfHeight);
			CheckSideColliding(m_colliders[i], 0.f, -m_colliders[i]->halfHeight);
			i++;
		}
	}
}

inline void Tilemap::CheckSideColliding(BoxCollider* collider, float offsetX, float offsetY) {
	int col, row;
	float tileX, tileY;
	float deltaX = 0.f;
	float deltaY = 0.f;
	bool isOutOfBounds = false;
	WCoordsToColRow(collider->position.x + offsetX, collider->position.y + offsetY, &col, &row);
	ColRowToWCoords(col, row, &tileX, &tileY);
	int tileID = m_pIDs[col + m_width* row];
	if (col < 0 || row < 0 || col >= m_width || row >= m_height) {
		return;
	}
	if ((m_collisionBitmask[tileID/32] >> (tileID % 32)) & 1) {
		if (offsetX != 0.f) {
			deltaX = collider->position.x + offsetX - tileX;
			printf("%d\n", deltaX);
			if (signbit(deltaX) != signbit(offsetX)) { //tells from where i'm measuring
				deltaX = 1.f - glm::abs(deltaX);
			} else {
				deltaX = glm::abs(deltaX);
			}
			deltaX = (signbit(offsetX) ? deltaX : -deltaX);
		}
		if (offsetY != 0.f) {
			deltaY = collider->position.y + offsetY - tileY;
			if (signbit(deltaY) != signbit(offsetY)) { 
				deltaY = 1.f - glm::abs(deltaY);
			} else {
				deltaY = glm::abs(deltaY);
			}
			deltaY = (signbit(offsetY) ? deltaY : -deltaY);
		}
	}
	if (deltaX != 0.f || deltaY != 0.f) {
		collider->pEntity->Translate(deltaX, deltaY, 0.f);
	}
}


void Tilemap::SetCollisionableTiles(int* frames, int count) {
	for (int i = 0; i < count; i++) {
		int frame = *(frames + i);
		m_collisionBitmask[frame/32] |= (1 << (frame%32));
	}
}

bool Tilemap::RegisterColliders(BoxCollider * collider) {
	for (int i = 0; i < MAX_COLLIDERS; i++) {
		if (m_colliders[i] == NULL) {
			m_colliders[i] = collider;
			m_collidersCount++;
			return true;
		}
	}
	return false;
}

void Tilemap::CalculateVertexPosition() {
	m_pCoords = new float[m_width * m_height * 4 * 3];
	float posX = 0.f;
	float posY = 0.f;
	int count = 0;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			m_pCoords[count*4*3] = posX;
			m_pCoords[count*4*3 + 1] = posY - m_tileSize;
			m_pCoords[count*4*3 + 2] = 0.f;
			m_pCoords[count*4*3 + 3] = posX;
			m_pCoords[count*4*3 + 4] = posY;
			m_pCoords[count*4*3 + 5] = 0.f;
			m_pCoords[count*4*3 + 6] = posX + m_tileSize;
			m_pCoords[count*4*3 + 7] = posY - m_tileSize;
			m_pCoords[count*4*3 + 8] = 0.f;
			m_pCoords[count*4*3 + 9] = posX + m_tileSize;
			m_pCoords[count*4*3 + 10] = posY;
			m_pCoords[count*4*3 + 11] = 0.f;
			count++;
			posX += m_tileSize;
		}
		posX = 0.f;
		posY -= m_tileSize;
	}
	m_vbPosition.SetData(m_pCoords, sizeof(float) * m_width * m_height * 4 * 3);
}

void Tilemap::CalculateUV() {
	unsigned int id = 0;
	m_pUVData = new float[m_width * m_height * 8];
	for (unsigned int i = 0; i < m_width * m_height; i++) {
		if (m_pIDs[i] == -1) {
			for (int j = 0; j < 8; j++) {
				m_pUVData[i * 8 + j] = -1;
			}
		} else {
			coords uvCoords = m_spriteSheet.GetSpritesUV(m_pIDs[i]);
			m_pUVData[i*8] = uvCoords.u0;
			m_pUVData[i*8 + 1] = uvCoords.v1;
			m_pUVData[i*8 + 2] = uvCoords.u0;
			m_pUVData[i*8 + 3] = uvCoords.v0;
			m_pUVData[i*8 + 4] = uvCoords.u1;
			m_pUVData[i*8 + 5] = uvCoords.v1;
			m_pUVData[i*8 + 6] = uvCoords.u1;
			m_pUVData[i*8 + 7] = uvCoords.v0;
		}
	}
	m_vbUV.SetData(m_pUVData, sizeof(float) * m_width * m_height * 8);
}

void Tilemap::CalculateVisibleTiles() {
	int count = 0;
	int tilesUntilCamX, tilesUntilCamY;
	WCoordsToColRow(m_cacheCamPosX - m_pRenderer->GetCameraWidth() * 0.5f, m_cacheCamPosY + m_pRenderer->GetCameraHeight() * 0.5f, &tilesUntilCamX, &tilesUntilCamY);
	for (int i = 0; i < m_visibleHeight; i++) {
		for (int j = 0; j < m_visibleWidth; j++) {
			if ((tilesUntilCamX + j < 0) || (tilesUntilCamY + i < 0) ) {
				m_pVisibleTiles[count] = -1;
			} else {
				m_pVisibleTiles[count] = tilesUntilCamX + j + (m_width*(i + tilesUntilCamY));
			}
			count++;
		}
	}
}

void Tilemap::WCoordsToColRow(float wcX, float wcY, int* col, int* row) {
	*col = (int)((wcX ) / m_tileSize);
	*row = (int)(-(wcY ) / m_tileSize);
}

void Tilemap::ColRowToWCoords(int col, int row, float* wcX, float* wcY){
	*wcX = (float)col*m_tileSize;
	*wcY = -(float)row*m_tileSize;
}

