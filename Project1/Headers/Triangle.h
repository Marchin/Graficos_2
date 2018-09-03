#pragma once
#include "Entity.h"
#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class ENGINEDLL_API Triangle {
public:
	Triangle(Material material, void* data, unsigned int size);
	~Triangle() {}
	void Draw();
private:
	VertexArray m_va;
	VertexBuffer m_vb;
	Material m_material;
};