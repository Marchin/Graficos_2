#include "../Headers/VertexBuffer.h"
#include "../Headers/Renderer.h"
#include <GLAD\glad.h>


VertexBuffer::VertexBuffer() {
	GLCall(glGenBuffers(1, &m_vb));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	GLCall(glGenBuffers(1, &m_vb));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_vb));
}

void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));
}

void VertexBuffer::Unbind() const{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int VertexBuffer::GetID() const {
	return m_vb;
}

void VertexBuffer::SetData(const void* data, unsigned int size) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}