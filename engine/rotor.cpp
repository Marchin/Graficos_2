inline Bivec3
wedge(const hmm_vec3 vec3A, const hmm_vec3 vec3B) {
    Bivec3 result = {
        vec3A.y*vec3B.z - vec3A.z*vec3B.y,
        vec3A.z*vec3B.x - vec3A.x*vec3B.z,
        vec3A.x*vec3B.y - vec3A.y*vec3B.x,
    };
	return result;
}

Rotor3
rotorFromVecAToVecB(const hmm_vec3 from, const hmm_vec3 to) {
    Rotor3 result;
    
    //result.a = HMM_DotVec3(from, to);
    result.bivec = wedge(from, to);
    
    return result;
}

#if 0
// TODO(Marchin): 
Rotor3
rotorFromEuler(hmm_vec3 vec) {
    f32 sine = 
}
#endif

Rotor3
rotorFromAngleAndBivec(f32 angleRadian, Bivec3 bivec) {
	Rotor3 result;
    
    f32 halfAngle = angleRadian*0.5f;
    f32 sinAngle = sinf(halfAngle);
    result.a = cosf(angleRadian);
    result.yz = bivec.yz * sinAngle;
    result.zx = bivec.zx * sinAngle;
    result.xy = bivec.xy * sinAngle;
    
    return result;
}

inline Rotor3 
operator*(Rotor3 a, Rotor3 b) {
	Rotor3 result;
	
    result.a = a.a*b.a - a.yz*b.yz - a.zx*b.zx + a.xy*b.xy;
    result.yz = a.a*b.yz + a.yz*b.a - a.zx*b.xy + a.xy*b.zx; 
    result.zx = a.a*b.zx + a.yz*b.xy + a.zx*b.a - a.xy*b.yz; 
    result.xy = a.a*b.xy - a.yz*b.zx + a.zx*b.yz + a.xy*b.a; 
    
	return result;
}

inline Rotor3 
&operator*=(Rotor3& rotA, Rotor3 rotB) {
    rotA = rotA * rotB;
	return rotA;
}



hmm_vec3
getRotatedVector(hmm_vec3 vec, Rotor3 rot) {
    //p = rot*vec
    hmm_vec3 p;
    
    p.x = rot.a*vec.x - rot.zx*vec.z + rot.xy*vec.y;
    p.y = rot.a*vec.y + rot.yz*vec.z - rot.xy*vec.x;
    p.z = rot.a*vec.z - rot.yz*vec.y + rot.zx*vec.x;
    
    f32 xyz = rot.yz*vec.x + rot.zx*vec.y + rot.xy*vec.z;
    
    //q = p*(rot*)
    hmm_vec3 q;
    
    q.x = p.x*rot.a + p.y*rot.xy - p.z*rot.zx + xyz*rot.yz;
    q.y = -p.x*rot.xy + p.y*rot.a + p.z*rot.yz + xyz*rot.zx;
    q.z = p.x*rot.zx - p.y*rot.yz + p.z*rot.a + xyz*rot.xy;
    
	return q;
}

inline Rotor3 
getRotatedRotor(Rotor3 rotated, Rotor3 rotor) {
	// should unwrap this for efficiency
	return rotor * rotated * rotorGetReverse(rotor);
}

// Equivalent to conjugate
inline Rotor3 
rotorGetReverse(Rotor3 rot) {
	return Rotor3{rot.a, -rot.yz, -rot.zx, -rot.xy};
}

inline f32 
rotorLengthSqrd(Rotor3* pRot) {
	return 1.f;
}

inline f32 
rotorLength(Rotor3* pRot) {
	return sqrtf(rotorLengthSqrd(pRot));
}

inline void 
rotorNormalize(Rotor3* pRot) {
	f32 length = rotorLength(pRot);
	pRot->a /= length; pRot->yz /= length; pRot->zx /= length; pRot->xy /= length;
}

inline hmm_mat4
getRotorMat4(Rotor3 rot) {
    hmm_vec3 vecX = getRotatedVector(VEC3_X, rot);
    hmm_vec3 vecY = getRotatedVector(VEC3_Y, rot);
    hmm_vec3 vecZ = getRotatedVector(VEC3_Z, rot);
    
    hmm_mat4 mat;
    mat.Rows[0] = { vecX[0], vecX[1], vecX[2], 0.f };
    mat.Rows[1] = { vecY[0], vecY[1], vecY[2], 0.f };
    mat.Rows[2] = { vecZ[0], vecZ[1], vecZ[2], 0.f };
    mat.Rows[3] = { 0.f, 0.f, 0.f, 1.f };
    
    return mat;
}