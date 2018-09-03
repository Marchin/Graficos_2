#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLAD\glad.h>
#include "ITexture.h"

namespace marchinGL {
	class ENGINEDLL_API Texture : public ITexture{
	public:
		Texture(unsigned int width, unsigned int height);
		Texture(const char* imgPath, const GLint  TextureWrap_S, const GLint  TextureWrap_T,
			const GLint  TextureMinFilter, const GLint  TextureMagFilter, const bool flipVertical = false);
		~Texture();
		void Bind(unsigned int slot) const override;
		unsigned int GetID() const override;
	private:
		inline GLenum AnalizeFormat(const char* imgPath);
		unsigned int m_texture;
	};
}
#endif //TEXTURE_H