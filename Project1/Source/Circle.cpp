#include "../Headers/Circle.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

Circle::Circle(Renderer* pRenderer, Material material, unsigned int sidesAmount)
	: m_sides(sidesAmount), m_material(material), m_pRenderer(pRenderer) {

	//Genero los strides para el VertexArray
	VertexBufferLayout layout;
	layout.Push<float>(3);
	SetSidesAmount(sidesAmount);
	m_va.AddBuffer(m_vb, layout);
}

void Circle::Draw() {
	m_material.Bind();
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	m_pRenderer->DrawBufferFan(0, m_sides + 2);
}

void Circle::SetVertices(void * data) {
}

void Circle::SetSidesAmount(unsigned int amount) {
	if (amount < 3) {
		amount = 3;
	} 
	m_sides = amount;
	float currAngle = 0.f;
	float rate = 2.f * 3.1415f / (float)amount;
	float* data = new float[(2 + amount) * 3];

	data[0] = 0.f;
	data[1] = 0.f;
	data[2] = 0.f;

	data[3] = 1.f;
	data[4] = 0.f;
	data[5] = 0.f;

	for (unsigned i = 0; i < amount; i++) {
		currAngle += rate;
		data[6 + (i * 3)] = cos(currAngle);
		data[7 + (i * 3)] = sin(currAngle);
		data[8 + (i * 3)] = 0.f;
	}

	m_vb.SetData(data, sizeof(float) * ((2 + amount) * 3));
	delete data;
}
