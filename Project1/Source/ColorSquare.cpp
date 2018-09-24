#include "../Headers/ColorSquare.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

ColorSquare::ColorSquare(Renderer * pRenderer, Material material, void* position, void* color) 
	: m_pRenderer(pRenderer), m_material(material), 
	m_vbPosition(position, 12*sizeof(float)), m_vbColor(color, 9 * sizeof(float)){


	VertexBufferLayout layout;
	layout.Push<float>(4);
	layout.Push<float>(3);

	m_va.AddBuffer(m_vbPosition, layout);
}


void ColorSquare::SetPoints(void* data) {
}

void ColorSquare::Draw(){
	m_material.Bind();
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	m_pEB->Bind();
	m_pRenderer->DrawElements(6);
}
