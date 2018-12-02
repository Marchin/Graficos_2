#pragma once
#include "Shape.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

class ENGINEDLL_API Circle : public Shape {
public:
	Circle(Renderer* pRenderer, Material material, unsigned int sidesAmount = 10, float radius = 1);
	~Circle() {}
	void Draw() override;
	void SetVertices(void* data);
	void SetSidesAmount(unsigned int amount);
	void SetRadius(float radius);
private:
	float m_radius;
	unsigned int m_sides;
	VertexArray m_va;
	VertexBuffer m_vb;
	Material m_material;
	Renderer* m_pRenderer;
};
