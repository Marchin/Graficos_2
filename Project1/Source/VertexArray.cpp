#include "../Headers/VertexArray.h"
#include "../Headers/Renderer.h"
#include "../Headers/VertexBufferLayout.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_va));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_va));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout) {
	Bind();
	vb.Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const VertexBufferElement& element = elements[i];
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
			layout.GetStride(), (const void*)offset));
		GLCall(glEnableVertexAttribArray(i));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_va));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
