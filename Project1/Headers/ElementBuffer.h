#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

class ElementBuffer {
public:
	ElementBuffer(const unsigned int* data, unsigned int count);
	~ElementBuffer();
	void Bind() const;
	void Unbind()const;
	unsigned int GetID() const;
	unsigned int GetCount() const;

private:
	unsigned int m_elementBufferObject;
	unsigned int m_count;
};

#endif // !ELEMENTBUFFER_H