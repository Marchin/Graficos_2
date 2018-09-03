#include "../Headers/Material.h"

Material::Material(Shader program) 
	: m_program(program) {

}

void Material::Bind() {
	m_program.Bind();
}
