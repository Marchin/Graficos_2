#include "../Headers/SpriteSheet.h"

SpriteSheet::SpriteSheet(Renderer * pRenderer, Material material, const char * texturePath, void * position, void * uv) 
	: Sprite(pRenderer,  material,  texturePath, position, uv), m_columns(0), m_rows(0) {
}

SpriteSheet::~SpriteSheet() {
	if (m_pUVCoords != nullptr) {
		delete m_pUVCoords;
	}
	if (m_pUVData != nullptr) {
		delete m_pUVData;
	}
}

void SpriteSheet::SetFrameSize(unsigned int width, unsigned int height) {
	m_frameWidth = width;
	if (height == 0) {
		m_frameHeight = m_frameWidth;
	} else {
		m_frameHeight = height;
	}
	SetupUV();
}

void SpriteSheet::SetFramesPerRow(unsigned int rows) {
	m_rows = rows;
}

void SpriteSheet::SetupUV() {
	if (m_columns == 0) {
		m_columns = m_texture.GetWidth() / m_frameWidth;
	}
	if (m_rows == 0) {
		m_rows = m_texture.GetHeight() / m_frameHeight;
	}
	if (m_pUVCoords != nullptr) {
		//delete m_pUVCoords;
	}
	m_pUVCoords = new coords[m_columns * m_rows];
	int id = 0;
	for (unsigned int iColumn = 0; iColumn < m_columns; iColumn++) {
		for (unsigned int iRow = 0; iRow < m_rows; iRow++) {
			float x = (float)(id % m_columns) * (float)m_frameWidth;
			float y = (float)(id / m_columns) * (float)m_frameHeight;
			m_pUVCoords[id].u0 = x / (float)m_texture.GetWidth();
			m_pUVCoords[id].u1 = (x + (float)m_frameWidth) / (float)m_texture.GetWidth();
			m_pUVCoords[id].v0 = (1 - y) / (float)m_texture.GetHeight();
			m_pUVCoords[id].v1 = (1 - (y + (float)m_frameHeight)) / (float)m_texture.GetHeight();
			id++;
		}
	}
	m_pUVData = new float[id * 8];
	for (unsigned int i = 0; i < id * 8; i += 8) {
		m_pUVData[id * 8] = m_pUVCoords[id].u0;
		m_pUVData[id * 8 + 1] = m_pUVCoords[id].v1;
		m_pUVData[id * 8 + 2] = m_pUVCoords[id].u0;
		m_pUVData[id * 8 + 3] = m_pUVCoords[id].v0;
		m_pUVData[id * 8 + 4] = m_pUVCoords[id].u1;
		m_pUVData[id * 8 + 5] = m_pUVCoords[id].v1;
		m_pUVData[id * 8 + 6] = m_pUVCoords[id].u1;
		m_pUVData[id * 8 + 7] = m_pUVCoords[id].v0;
	}
	float frame0[8] = {
		m_pUVData[0],m_pUVData[1],m_pUVData[2],m_pUVData[3],
		m_pUVData[4],m_pUVData[5],m_pUVData[6],m_pUVData[7]
	};
	m_vbUV.SetData(frame0, sizeof(frame0));
}
