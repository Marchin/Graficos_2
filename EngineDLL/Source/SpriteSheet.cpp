#include "../Headers/SpriteSheet.h"

SpriteSheet::SpriteSheet(Renderer * pRenderer, Material material, const char * texturePath, void * position, void * uv) 
	: Sprite(pRenderer,  material,  texturePath, position, uv), m_columns(0), m_rows(0), m_pUVCoords(0), m_pUVData(0) {
}

SpriteSheet::~SpriteSheet() {
	if (m_pUVCoords != 0) {
		delete[] m_pUVCoords;
	}
	if (m_pUVData != 0) {
		delete[] m_pUVData;
	}
}

void SpriteSheet::SetFrame(unsigned int frame) {
	float buff[8] = {
		m_pUVData[(frame * 8) + 0],m_pUVData[(frame * 8) + 1],
		m_pUVData[(frame * 8) + 2],m_pUVData[(frame * 8) + 3],
		m_pUVData[(frame * 8) + 4],m_pUVData[(frame * 8) + 5],
		m_pUVData[(frame * 8) + 6],m_pUVData[(frame * 8) + 7]
	};
	m_vbUV.SetData(buff, sizeof(buff));
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

coords SpriteSheet::GetSpritesUV(int frame) {
	return m_pUVCoords[frame];
}

void SpriteSheet::SetupUV() {
	if (m_columns == 0) {
		m_columns = m_texture.GetWidth() / m_frameWidth;
	}
	if (m_rows == 0) {
		m_rows = m_texture.GetHeight() / m_frameHeight;
	}
	if (m_pUVCoords != 0) {
		delete[] m_pUVCoords;
		m_pUVCoords = 0;
	}
	m_pUVCoords = new coords[m_columns * m_rows];
	int id = 0;
	for (unsigned int iRow = 0; iRow < m_rows; iRow++) {
		for (unsigned int iColumn = 0; iColumn < m_columns; iColumn++) {
			float x = (float)(id % m_columns) * (float)m_frameWidth;
			float y = (float)(id / m_columns) * (float)m_frameHeight;
			m_pUVCoords[id].u0 = x / (float)m_texture.GetWidth();
			m_pUVCoords[id].u1 = (x + (float)m_frameWidth) / (float)m_texture.GetWidth();
			m_pUVCoords[id].v0 = 1.f - (y / (float)m_texture.GetHeight()) ;
			m_pUVCoords[id].v1 = 1.f - ((y + (float)m_frameHeight) / (float)m_texture.GetHeight()); //da negativo
			id++;
		}
	}
	if (m_pUVData != 0) {
		delete m_pUVCoords;
		m_pUVData = 0;
	}
	m_pUVData = new float[id * 8];
	for (unsigned int i = 0; i < id; i++) {
		m_pUVData[i * 8] = m_pUVCoords[i].u0;
		m_pUVData[i * 8 + 1] = m_pUVCoords[i].v1;
		m_pUVData[i * 8 + 2] = m_pUVCoords[i].u0;
		m_pUVData[i * 8 + 3] = m_pUVCoords[i].v0;
		m_pUVData[i * 8 + 4] = m_pUVCoords[i].u1;
		m_pUVData[i * 8 + 5] = m_pUVCoords[i].v1;
		m_pUVData[i * 8 + 6] = m_pUVCoords[i].u1;
		m_pUVData[i * 8 + 7] = m_pUVCoords[i].v0;
	}
	SetFrame(0);
}
