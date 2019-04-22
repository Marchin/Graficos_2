ENGINE_API inline Bivec3
wedge(const hmm_vec3 vec3A, const hmm_vec3 vec3B) {
    Bivec3 result = {
        vec3A.y*vec3B.z - vec3A.z*vec3B.y,
        vec3A.z*vec3B.x - vec3A.x*vec3B.z,
        vec3A.x*vec3B.y - vec3A.y*vec3B.x,
    };
	return result;
}

ENGINE_API Rotor3
rotorFromVecAToVecB(const hmm_vec3 from, const hmm_vec3 to) {
    Rotor3 result;
    
    //result.a = HMM_DotVec3(from, to);
    result.bivec = wedge(from, to);
    
    return result;
}

ENGINE_API Rotor3
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

ENGINE_API inline Rotor3 
operator*(Rotor3 rotA, Rotor3 rotB) {
	Rotor3 result;
	
	return result;
}

ENGINE_API inline Rotor3 
&operator*=(Rotor3& rotA, Rotor3 rotB) {
    rotA = rotA * rotB;
	return rotA;
}

ENGINE_API hmm_vec3
getRotatedVector(hmm_vec3 vec, Rotor3 rot) {
    
	return result;
}

ENGINE_API inline Rotor3 
getRotatedRotor(Rotor3 rotated, Rotor3 rotor) {
	// should unwrap this for efficiency
	return rotor * rotated * rotorGetReverse(rotor);
}

// Equivalent to conjugate
ENGINE_API inline Rotor3 
rotorGetReverse(Rotor3 rot) {
	return Rotor3{rot.a, -rot.xy, -rot.xz, -rot.yz};
}

ENGINE_API inline f32 
rotorLengthSqrd(Rotor3* pRot) {
	return 1.f;
}

ENGINE_API inline f32 
rotorLength(Rotor3* pRot) {
	return sqrtf(rotorLengthSqrd(pRot));
}

ENGINE_API inline void 
rotorNormalize(Rotor3* pRot) {
	f32 length = rotorLength(pRot);
	pRot->a /= length; pRot->xy /= length; pRot->xz /= length; pRot->yz /= length;
}
