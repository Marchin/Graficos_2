#version 330 core

in vec2 vTexCoords;

out vec4 fColor;

//uniform sampler2D texture_diffuse;

void main() {  
    if (vTexCoords.x < 0.0f  || vTexCoords.y < 0.0f) {
        discard;
    }  
    fColor = vec4(0.5f, 0.5f, 0.5f, 0.5f);
}