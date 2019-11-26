#version 430
#define STRIDE 16
#define BAND_AMOUNT (256/STRIDE)
#define BAND_AMOUNT_FLOAT (256.f/float(STRIDE))
#define DELAY_AMOUNT 8
#define TOTAL (BAND_AMOUNT*DELAY_AMOUNT)

layout (location = 0) in vec3 aPos;

struct Complex {
    double r, i;
};

layout(std430, binding = 6) buffer ComB {
    float b[ ]; //Frequency buff
};

out vec4 vColor;

uniform vec4 colorLFN;
uniform vec4 colorHFN;
uniform vec4 colorLFP;
uniform vec4 colorHFP;
uniform vec4 borderColor;
uniform float scale;
uniform float height;
uniform bool isBorder;
uniform bool hasFakeTransparency;
uniform mat4 viewProj;

void main() {
    float mul = 0.00002f/(15.f/height);
    int id = TOTAL - gl_InstanceID - 1;
    float value = min(height, b[id]*mul);
    int mod = id%(BAND_AMOUNT);
    int div = id/(BAND_AMOUNT);
    float modNorm = mod/(BAND_AMOUNT_FLOAT - 1.f);
    float divNorm = div/(BAND_AMOUNT_FLOAT - 1.f);
    
    mat4 model;
    model[0][0] = scale;
    model[1][1] = scale;
    model[2][2] = scale;
    
    model[3] = vec4(mod*0.2f - 3f, value, -div*0.2f, 1.f);
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