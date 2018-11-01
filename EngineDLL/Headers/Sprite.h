#pragma once
#include "Shape.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "Texture.h"

class ENGINEDLL_API Sprite : public Shape {
public:
	Sprite(Renderer* pRenderer, Material material, const char* texturePath, void* position, void* uv);
	~Sprite() {}
	void SetVertices(void* position) override;
	void SetUV(void* uvCoords);
	void Draw() override;
protected:
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbUV;
	marchinGL::Texture m_texture;
	Material m_material;
	Renderer* m_pRenderer;
};