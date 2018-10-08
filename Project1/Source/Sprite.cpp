#include "../Headers/Sprite.h"
#include "../Headers/VertexBufferLayout.h"

Sprite::Sprite(Renderer * pRenderer, Material material, const char* texturePath, void * position, void * uv)
	: m_pRenderer(pRenderer), m_material(material), m_texture(texturePath, true),
	m_vbPosition(position, 12 * sizeof(float)), m_vbUV(uv, 8 * sizeof(float)) {

	//Genero los strides para el VertexArray
	VertexBufferLayout layoutPosition, layoutUV;
	layoutPosition.Push<float>(3);
	layoutUV.Push<float>(2);
	m_material.Bind();
	m_texture.Bind(0);
	material.SetIntProperty("tex",0);

	m_va.AddBufferByLocation(m_vbPosition, layoutPosition, 0);
	m_va.AddBufferByLocation(m_vbUV, layoutUV, 1);
}

void Sprite::SetVertices(void * position) {
	m_vbPosition.SetData(position, 12 * sizeof(float));
}

void Sprite::SetUV(void * uvCoords){
	m_vbUV.SetData(uvCoords, 8 * sizeof(float));
}

void Sprite::Draw(){
	m_material.Bind();
	m_pRenderer->SetModelMatrix(GetModelMatrix());
	m_material.SetMatrixProperty("uModelViewProjection", m_pRenderer->GetModelViewProj());
	m_va.Bind();
	m_pRenderer->DrawBufferStrip(0, 4);
}
