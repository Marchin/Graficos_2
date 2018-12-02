#include "../Headers/Tilemap.h"
#include "../Headers/TilemapLoader.h"
#include "../Headers/VertexBufferLayout.h"

Tilemap::Tilemap(const char* fileDir, SpriteSheet* pSpriteSheet,
	Renderer* pRenderer, Material material)
	: m_spriteSheet(*pSpriteSheet), m_pRenderer(pRenderer), m_material(material),
	m_width(0), m_height(0){

	TilemapLoader loader(fileDir, &m_ids, &m_width, &m_height);


	//Genero los strides para el VertexArray
	VertexBufferLayout layoutPosition, layoutUV;
	layoutPosition.Push<float>(3);
	layoutUV.Push<float>(2);
	m_material.Bind();
	m_spriteSheet.GetTexture()->Bind(0);
	material.SetIntProperty("tex",0);

	CalculateVertexPosition();
	CalculateUV();

	m_va.AddBufferByLocation(m_vbPosition, layoutPosition, 0);
	m_va.AddBufferByLocation(m_vbUV, layoutUV, 1);
}

Tilemap::~Tilemap() {
	if (m_ids != NULL) {
		delete[] m_ids;
	}
	if (m_pUVData != NULL) {
		delete[] m_pUVData;
	}
}

void Tilemap::Draw() {
	m_material.Bind();
	m_spriteSheet.GetTexture()->Bind(0);
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			m_pRenderer->DrawBufferStrip((j + (m_width*i)) * 4, 4);
		}
	}
}

void Tilemap::CalculateVertexPosition() {
	float* coords = new float[m_width * m_height * 4 * 3];
	float posX = 0.f;
	float posY = 0.f;
	
	int count = 0;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			coords[count*4*3] = posX;
			coords[count*4*3 + 1] = posY - 1.f;
			coords[count*4*3 + 2] = 0.f;
			coords[count*4*3 + 3] = posX;
			coords[count*4*3 + 4] = posY;
			coords[count*4*3 + 5] = 0.f;
			coords[count*4*3 + 6] = posX + 1.f;
			coords[count*4*3 + 7] = posY - 1.f;
			coords[count*4*3 + 8] = 0.f;
			coords[count*4*3 + 9] = posX + 1.f;
			coords[count*4*3 + 10] = posY;
			coords[count*4*3 + 11] = 0.f;
			count++;
			posX += 1.f;
		}
		posX = 0.f;
		posY -= 1.f;
	}
	m_vbPosition.SetData(coords, sizeof(float) * m_width * m_height * 4 * 3);
	int a = sizeof(coords);
}

void Tilemap::CalculateUV() {
	unsigned int id = 0;
	m_pUVData = new float[m_width * m_height * 8];
	for (unsigned int i = 0; i < m_width * m_height; i++) {
		if (m_ids[i] == -1) {
			for (int j = 0; j < 8; j++) {
				m_pUVData[i * 8 + j] = -1;
			}
		} else {
			coords uvCoords = m_spriteSheet.GetSpritesUV(m_ids[i]);
			m_pUVData[i * 8] = uvCoords.u0;
			m_pUVData[i * 8 + 1] = uvCoords.v1;
			m_pUVData[i * 8 + 2] = uvCoords.u0;
			m_pUVData[i * 8 + 3] = uvCoords.v0;
			m_pUVData[i * 8 + 4] = uvCoords.u1;
			m_pUVData[i * 8 + 5] = uvCoords.v1;
			m_pUVData[i * 8 + 6] = uvCoords.u1;
			m_pUVData[i * 8 + 7] = uvCoords.v0;
		}
	}
	m_vbUV.SetData(m_pUVData, sizeof(float) * m_width * m_height * 8);
	int a = sizeof(m_pUVData);
}
