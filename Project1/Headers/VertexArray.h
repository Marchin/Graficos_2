#ifndef VERTEXARRAY_H
#define	VERTEXARRAY_H

#include "VertexBuffer.h"
#include "Export.h"

class VertexBufferLayout;

class ENGINEDLL_API VertexArray {
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_va;
};


#endif // !VERTEXARRAY_H
