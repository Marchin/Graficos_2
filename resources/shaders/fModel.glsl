#version 330 core

in vec2 vTexCoords;

out vec4 fColor;

uniform sampler2D texture_diffuse1;

void main() {
	/*
    vec4 hello = texture(texture_diffuse1, vTexCoords);
	vec2 a = gl_FragCoord.xy;
	a.x /= 800.f;
	a.y /= 600.f;
    */
    fColor = texture(texture_diffuse1, vTexCoords);
}