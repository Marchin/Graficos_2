#version 330 core

in vec3 vNormal;
in vec2 vTexCoords;

out vec4 fColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {    
  vec4 waste = texture(texture_specular1, vTexCoords);
  fColor = texture(texture_diffuse1, vTexCoords);
}