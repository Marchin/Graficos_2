#include "../Headers/TextureController.h"
#include "../Headers/Renderer.h"
#include <iostream>
#define UINT_MAX 4294967295

namespace marchinGL {
	TextureController::TextureController()
		: m_count(0), m_previousShader(UINT_MAX) {
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextures);
	}

	TextureController::~TextureController() {
	}

	void TextureController::AddTexture(const ITexture* texture, Shader& shader, const char* sampler) {
		if (m_previousShader == UINT_MAX) {
			m_previousShader = shader.ID;
		} else if (m_previousShader != shader.ID) {
			m_count = 0;
			m_previousShader = shader.ID;
		}
		if ((int)m_count < m_maxTextures) {
			texture->Bind(m_count);
			shader.Bind();
			shader.SetInt(sampler, m_count++);
		} else {
			std::cout << "No more texture slots available" << std::endl;
		}
	}

	unsigned int TextureController::GetCount() const {
		return m_count;
	}

	void TextureController::SetSlot(unsigned int slot) {
		m_count = slot;
	}

	void TextureController::Reset() {
		m_count = 0;
	}
}