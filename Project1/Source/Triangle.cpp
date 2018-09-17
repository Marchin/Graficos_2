#include "../Headers/Triangle.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

Triangle::Triangle(Renderer* pRenderer, Material material, void* data, unsigned int size)
	:m_material(material), m_vb(data, size), m_pRender(pRenderer) {

	VertexBufferLayout layout;
	layout.Push<float>(3);
	m_va.AddBuffer(m_vb, layout);
}

void Triangle::Draw() {
	m_material.Bind();
	m_va.Bind();
	m_pRender->DrawTriangles(0, 3);
}