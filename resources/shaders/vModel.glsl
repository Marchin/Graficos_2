#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

uniform mat4 uModelViewProjection;

void main() {
    vTexCoords = aTexCoords;    
    gl_Position = uModelViewProjection * vec4(aPos, 1.0);
}