#version 430

#define SIZE 512
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
}