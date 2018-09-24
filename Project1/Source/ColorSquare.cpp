#include "../Headers/ColorSquare.h"
#include "../Headers/VertexBufferLayout.h"
#include <glfw3.h>

ColorSquare::ColorSquare(Renderer * pRenderer, Material material, void* position, void* color) 
	: m_pRenderer(pRenderer), m_material(material), 
	m_vbPosition(position, 12 * sizeof(float)), m_vbColor(color, 12 * sizeof(float)){


	VertexBufferLayout layout;
	layout.Push<float>(4);
	layout.Push<float>(4);

	m_va.AddBufferByLocation(m_vbPosition, layout, 0);
	m_va.AddBufferByLocation(m_vbColor, layout, 1);
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
