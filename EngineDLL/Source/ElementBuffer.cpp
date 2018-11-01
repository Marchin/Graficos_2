#include "../Headers/ElementBuffer.h"
#include "../Headers/Renderer.h"
#include <GLAD\glad.h>

ElementBuffer::ElementBuffer(const unsigned int* data, unsigned int count) 
	:	m_count(count) {

	GLCall(glGenBuffers(1, &m_elementBufferObject));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

ElementBuffer::~ElementBuffer() {
	GLCall(glDeleteBuffers(1, &m_elementBufferObject));
}

void ElementBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject));
}

void ElementBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int ElementBuffer::GetID() const {
	return m_elementBufferObject;
}

unsigned int ElementBuffer::GetCount() const {
	return m_count;
}
