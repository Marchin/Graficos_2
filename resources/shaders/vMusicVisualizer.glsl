#version 330 core
#define BAND_AMOUNT 8
#define BAND_AMOUNT_FLOAT 8.f
#define DELAY_AMOUNT 7
#define TOTAL BAND_AMOUNT*DELAY_AMOUNT

layout (location = 0) in vec3 aPos;

out vec4 vColor;

uniform float scale;
uniform float band[TOTAL];
uniform int isWhite;
uniform mat4 viewProj;

void main() {
    mat4 model;
    model[0][0] = scale;
    model[1][1] = scale;
    model[2][2] = scale;
    float mul = 0.00003f;
    float value = min(15.f, band[gl_InstanceID]*mul);
    int id = TOTAL - gl_InstanceID - 1;
    int mod = id%BAND_AMOUNT;
    int div = id/BAND_AMOUNT;
    model[3] = vec4(mod - 3.5f, value, -div, 1.f);
    gl_Position = viewProj * model * vec4(aPos, 1.f);
    if (isWhite > 0) {
        vColor = vec4(1.f, 1.f, 1.f, 1.f);
    } else {
        float color = (div)/BAND_AMOUNT_FLOAT;
        vec4 diff = vec4(color, color, color, 0.f)/1.5f;
        vColor = vec4(1.f - color, (mod + 1)/BAND_AMOUNT_FLOAT, color, 1.f) - diff;
        
    }
}