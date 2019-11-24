#version 430
#define BAND_AMOUNT 8
#define BAND_AMOUNT_FLOAT 8.f
#define DELAY_AMOUNT 8
#define TOTAL BAND_AMOUNT*DELAY_AMOUNT

layout (location = 0) in vec3 aPos;

out vec4 vColor;

uniform vec4 colorLFN;
uniform vec4 colorHFN;
uniform vec4 colorLFP;
uniform vec4 colorHFP;
uniform vec4 borderColor;
uniform float scale;
uniform float height;
uniform float band[TOTAL];
uniform bool isBorder;
uniform bool hasFakeTransparency;
uniform mat4 viewProj;

void main() {
    float mul = 0.00008f/(15.f/height);
    float value = min(height, band[gl_InstanceID]*mul);
    int id = TOTAL - gl_InstanceID - 1;
    int mod = id%BAND_AMOUNT;
    int div = id/BAND_AMOUNT;
    float modNorm = mod/(BAND_AMOUNT_FLOAT - 1.f);
    float divNorm = div/(BAND_AMOUNT_FLOAT - 1.f);
    
    mat4 model;
    model[0][0] = scale;
    model[1][1] = scale;
    model[2][2] = scale;
    
    model[3] = vec4(mod - 3.5f, value, -div, 1.f);
    gl_Position = viewProj * model * vec4(aPos, 1.f);
    vec4 colorN = (1.f-modNorm)*colorLFN + modNorm*colorHFN;
    vec4 colorP = (1.f-modNorm)*colorLFP + modNorm*colorHFP;
    vec4 finalColor = (1.f-divNorm)*colorN + divNorm*colorP;
    if (isBorder) {
        vColor = borderColor;
    } else if (hasFakeTransparency) {
        vec4 fakeTransparency = vec4(divNorm/2.f,divNorm/1.5f,divNorm, 0.f)/1.5f;
        vColor = finalColor/(1.f + divNorm);
    } else {
        vColor = finalColor;
    }
}