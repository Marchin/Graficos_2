#pragma once
#include "Entity.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

class ENGINEDLL_API ColorSquare : public Entity {
public:
	ColorSquare(Renderer* pRenderer, Material material, void* position, void* color);
	~ColorSquare() {}
	void SetPoints(void* position);
	void SetColors(void* color);
	void Draw();
private:
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbColor;

	Material m_material;
	Renderer* m_pRenderer;
};