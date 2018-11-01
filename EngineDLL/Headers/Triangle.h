#pragma once
#include "Shape.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

class ENGINEDLL_API Triangle : public Shape {
public:
	Triangle(Renderer* pRenderer, Material material, void* data, unsigned int size);
	~Triangle() {}
	void Draw() override;
	void SetVertices(void* data) override;
private:
	VertexArray m_va;
	VertexBuffer m_vb;
	Material m_material;
	Renderer* m_pRenderer;
};