#version 430

#define SIZE 512
#define DELAY_AMOUNT 8
#define STRIDE 16
#define BITCOUNT 9

struct Complex {
    double r, i;
};

layout(std140, binding = 4) buffer ComI {
    Complex f[ ]; //Input buff
};

layout(std140, binding = 5) buffer ComT {
    Complex t[ ]; //Twiddle buff
};

layout(std430, binding = 6) buffer ComB {
    float b[ ]; //Frequency buff
};

layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;

Complex mul(Complex a, Complex b) { 
    return Complex(a.r*b.r - a.i*b.i, a.r*b.i + a.i*b.r);
}

Complex sum(Complex a, Complex b) { 
    return Complex(a.r + b.r, a.i + b.i);
}

void main() {
    int halfPow2 = 1;
    int stride = 2;
    int halfStride = stride/2;
    int halfSize = SIZE/2;
    for (int i = 0; i < BITCOUNT; ++i) {
        int offset = SIZE/stride;
        for (int j = 0; j < SIZE; j += stride) {
            int twiddleSelector = 0;
            for (int k = 0; k < halfStride; ++k) {
                int iF = j + k;
                Complex aux = f[iF];
                f[iF] = sum(aux, 
                            mul(t[twiddleSelector],
                                f[iF + halfStride]));
                f[iF + halfStride] = sum(aux,
                                         mul(t[twiddleSelector+halfSize],
                                             f[iF + halfStride]));
                twiddleSelector += offset;
            }
        }
        stride *= 2;
        halfStride *= 2;
        halfPow2 *= 2;
    }
    
    float mul = 1.f/float(STRIDE);
    int singleRowBandCount = halfSize/STRIDE;
    int bandCount = singleRowBandCount*DELAY_AMOUNT;
    int oldBandCount = bandCount - singleRowBandCount;
    
    for (int iBand = oldBandCount - 1; iBand >= 0; --iBand) {
        b[iBand + singleRowBandCount] = b[iBand];
    }
    
    for (int iBand = 0, count = 0; iBand < halfSize; iBand += STRIDE, ++count) {
        b[count] = 0.f;
        for (int i = 0; i < STRIDE; ++i) {
            b[count] += float(sqrt(f[iBand+i].r*f[iBand+i].r +
                                   f[iBand+i].i*f[iBand+i].i))*mul;
        }
    }
    
    b[0] *= 0.5f;
    for (int i = 0; i < 4; ++i) {
        b[i] *= 0.5f;
    }
}