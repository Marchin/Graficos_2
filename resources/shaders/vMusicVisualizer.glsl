#version 430
#define STRIDE 16
#define BAND_AMOUNT (256/STRIDE)
#define BAND_AMOUNT_FLOAT (256.f/float(STRIDE))
#define DELAY_AMOUNT 8
#define TOTAL (BAND_AMOUNT*DELAY_AMOUNT)

struct Complex {
    double r, i;
};

layout(std430, binding = 6) buffer ComB {
    float b[ ]; //Frequency buff
};

out VS_OUT {
    vec4 color;
} vs_out;

uniform vec4 colorLFN;
uniform vec4 colorHFN;
uniform vec4 colorLFP;
uniform vec4 colorHFP;
uniform vec4 borderColor;
uniform float height;
uniform bool isBorder;
uniform bool hasFakeTransparency;

void main() {
    float mul = 0.0002f/(15.f/height);
    int id = TOTAL - gl_InstanceID - 1;
    float value = min(height, b[id]*mul);
    int mod = id%(BAND_AMOUNT);
    int div = id/(BAND_AMOUNT);
    float modNorm = mod/(BAND_AMOUNT_FLOAT - 1.f);
    float divNorm = div/(BAND_AMOUNT_FLOAT - 1.f);
    
    gl_Position = vec4(mod - 7.f, value, -div, 1.f);
    vec4 colorN = (1.f-modNorm)*colorLFN + modNorm*colorHFN;
    vec4 colorP = (1.f-modNorm)*colorLFP + modNorm*colorHFP;
    vec4 finalColor = (1.f-divNorm)*colorN + divNorm*colorP;
    if (isBorder) {
        vs_out.color = borderColor;
    } else if (hasFakeTransparency) {
        vec4 fakeTransparency = vec4(divNorm/2.f,divNorm/1.5f,divNorm, 0.f)/1.5f;
        vs_out.color = finalColor/(1.f + divNorm);
    } else {
        vs_out.color = finalColor;
    }
}