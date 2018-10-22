#include "../Headers/SpriteSheet.h"

SpriteSheet::SpriteSheet(Renderer * pRenderer, Material material, const char * texturePath, void * position, void * uv) 
	: Sprite(pRenderer,  material,  texturePath, position, uv), m_framesPerColumn(0), m_framesPerRow(0) {
}

void SpriteSheet::SetFrameSize(unsigned int width, unsigned int height) {
	m_frameWidth = width;
	if (height == 0) {
		m_frameHeight = m_frameWidth;
	} else {
		m_frameHeight = height;
	}
}

void SpriteSheet::SetupUV() {
	if (m_framesPerColumn == 0) {
		m_framesPerColumn = m_texture.GetHeight() / m_frameHeight;
	}
	if (m_framesPerRow == 0) {
		m_framesPerRow = m_texture.GetWidth() / m_frameWidth;
	}
}
