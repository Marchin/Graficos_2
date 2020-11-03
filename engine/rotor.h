#ifndef ROTOR_H

//http://marctenbosch.com/quaternions/

struct Bivec3 {
    union {
        struct {
            float yz;
            float zx;
            float xy;
        };
        struct {
            float dx;
            float dy;
            float dz;
        };
        hmm_vec3 vec;
    };
};

struct Rotor3 {
    float a;
    
    union {
        struct {
            float yz;
            float zx;
            float xy;
        };
        struct {
            float dx;
            float dy;
            float dz;
        };
        Bivec3 bivec;
        hmm_vec3 vec;
    };
};

global const Bivec3 BivecXY = Bivec3{1, 0, 0};
global const Bivec3 BivecYZ = Bivec3{0, 1, 0};
global const Bivec3 BivecZX = Bivec3{0, 0, 1};

global const Rotor3 RotorYZ = Rotor3{0, 1, 0, 0};
global const Rotor3 RotorZX = Rotor3{0, 0, 1, 0};
global const Rotor3 RotorXY = Rotor3{0, 0, 0, 1};

ENGINE_API inline Bivec3 wedge(const hmm_vec3 vec3A, const hmm_vec3 vec3B);
ENGINE_API Rotor3 rotorFromVecAToVecB(const hmm_vec3 from, const hmm_vec3 to);
ENGINE_API Rotor3 rotorFromAngleAndBivec(Bivec3 bivec, float angleRadian);
ENGINE_API inline Rotor3 operator*(Rotor3 rotA, Rotor3 rotB);
ENGINE_API hmm_vec3 getRotatedVector(hmm_vec3 vec, Rotor3 rot);
ENGINE_API inline Rotor3 &operator*=(Rotor3& rotA, Rotor3 rotB);
ENGINE_API inline Rotor3 getRotatedRotor(Rotor3 rotated, Rotor3 rotor);
ENGINE_API inline Rotor3 rotorGetReverse(Rotor3 rot);
ENGINE_API Rotor3 rotorFromAngleAndAxis(f32 angleRadian, hmm_vec3 axis);
ENGINE_API inline f32 rotorLengthSqrd(Rotor3 rot);
ENGINE_API inline f32 rotorLength(Rotor3 rot);
//ENGINE_API inline void rotorNormalize(Rotor3 rot);

#define ROTOR_H
#endif //ROTOR_H