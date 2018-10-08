#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLAD/glad.h>
#include "ITexture.h"

namespace marchinGL {
	class ENGINEDLL_API Texture : public ITexture{
	public:
		Texture(unsigned int width, unsigned int height);
		Texture(const char* imgPath, const bool flipVertical = false,
			const GLint  TextureWrap_S = GL_CLAMP_TO_EDGE, const GLint  TextureWrap_T = GL_CLAMP_TO_EDGE,
			const GLint  TextureMinFilter = GL_NEAREST, const GLint  TextureMagFilter = GL_NEAREST);
		~Texture();
		void Bind(unsigned int slot) const override;
		unsigned int GetID() const override;
	private:
		unsigned int m_texture;
	};
}
#endif //TEXTURE_H