#pragma once
#include "Entity.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

class ENGINEDLL_API Triangle : public Entity {
public:
	Triangle(Renderer* pRenderer, Material material, void* data, unsigned int size);
	~Triangle() {}
	void Draw();
private:
	VertexArray m_va;
	VertexBuffer m_vb;
	Material m_material;
	Renderer* m_pRenderer;
};