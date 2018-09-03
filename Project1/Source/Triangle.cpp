#include "../Headers/Triangle.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

Triangle::Triangle(Material material, void* data, unsigned int size) 
	:m_material(material), m_vb(data, size) {

	VertexBufferLayout layout;
	layout.Push<float>(3);
	m_va.AddBuffer(m_vb, layout);
}

void Triangle::Draw() {
	m_material.Bind();
	m_va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
