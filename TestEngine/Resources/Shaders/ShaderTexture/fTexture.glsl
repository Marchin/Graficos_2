#version 330 core

in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fColor;

uniform sampler2D tex;

void main() {    
  fColor = texture(tex, vTexCoords);
}