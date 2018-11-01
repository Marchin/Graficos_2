#pragma once
#include "Shape.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

class ENGINEDLL_API ColorSquare : public Shape {
public:
	ColorSquare(Renderer* pRenderer, Material material, void* position, void* color);
	~ColorSquare() {}
	void SetVertices(void* position) override;
	void SetColors(void* color);
	void Draw() override;
private:
	VertexArray m_va;
	VertexBuffer m_vbPosition;
	VertexBuffer m_vbColor;
	Material m_material;
	Renderer* m_pRenderer;
};