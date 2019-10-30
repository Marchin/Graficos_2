#version 330 core

layout (location = 0) in vec3 aPos;

uniform float band[8];
uniform mat4 viewProj;

void main() {
    mat4 model;
    model[0][0] = 1.f;
    model[1][1] = 1.f;
    model[2][2] = 1.f;
    float mul = 0.001f;
    float value = min(10.f, band[gl_InstanceID]*mul);
    model[3] = vec4(gl_InstanceID, value, 0.f, 1.f);
    gl_Position = viewProj * model * vec4(aPos, 1.f);
    //vPos = projection * view * model * vec4(aPos, 1.f);
}