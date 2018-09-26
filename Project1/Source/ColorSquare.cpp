#include "../Headers/ColorSquare.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

ColorSquare::ColorSquare(Renderer * pRenderer, Material material, void* position, void* color) 
	: m_pRenderer(pRenderer), m_material(material), 
	m_vbPosition(position, 12 * sizeof(float)), m_vbColor(color, 12 * sizeof(float)){


	VertexBufferLayout layoutPosition, layoutColor;
	layoutPosition.Push<float>(3);
	layoutColor.Push<float>(3);

	m_va.AddBufferByLocation(m_vbPosition, layoutPosition, 0);
	//m_va.AddBuffer(m_vbPosition, layoutPosition);
	m_va.AddBufferByLocation(m_vbColor, layoutColor, 1);
}

void ColorSquare::SetPoints(void* position) {
	m_vbPosition.SetData(position, 12 * sizeof(float));
}

void ColorSquare::SetColors(void * color) {
	m_vbColor.SetData(color, 12 * sizeof(float));
}

void ColorSquare::Draw(){
	m_material.Bind();
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	m_pRenderer->DrawBuffer(0, 4);
}
