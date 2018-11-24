#include "../Headers/Animation.h"

Animation::Animation(SpriteSheet* pSpriteSheet, const unsigned int* frames, unsigned int count)
	: m_interval(1.f/3.f), m_pSpriteSheet(pSpriteSheet), m_counter(0.f), m_currentFrame(0), m_count(count) {

	m_frames = new unsigned int[count];
	memcpy(m_frames, frames, count * sizeof(unsigned int));
}

Animation::~Animation() {
	delete m_frames;
}

void Animation::Update(float deltaTime) {
	m_counter += deltaTime;
	if (m_counter >= m_interval) {
		m_currentFrame++;
		m_currentFrame %= m_count;
		m_pSpriteSheet->SetFrame(*(m_frames + m_currentFrame));
		m_counter = 0.f;
	}
}

void Animation::SetFPS(unsigned int fps) {
	m_interval = 1.f / (float)fps;
}
