#ifndef TEXTURECONTOLLER_H
#define TEXTURECONTOLLER_H

#include "Texture.h"
//#include "Cubemap.h"
#include "Shader.h"

namespace marchinGL {
	class ENGINEDLL_API TextureController {
	public:
		TextureController();
		~TextureController();
		void AddTexture(const ITexture* texture, Shader& shader, const char* sampler);
		void Reset();
		unsigned int GetCount() const;
		void SetSlot(unsigned int slot);
	private:
		unsigned int m_previousShader;
		unsigned int m_count;
		int m_maxTextures;
	};
}
#endif // !TEXTURECONTOLLER_H

