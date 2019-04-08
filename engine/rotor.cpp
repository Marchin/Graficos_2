ENGINE_API inline Bivec3
wedge(const hmm_vec3 vec3A, const hmm_vec3 vec3B) {
    Bivec3 result = {
		vec3A.x*vec3B.y - vec3A.y*vec3B.x,
		vec3A.x*vec3B.z - vec3A.z*vec3B.x,
		vec3A.y*vec3B.z - vec3A.z*vec3B.y 
    };
	return result;
}

ENGINE_API Rotor3
rotorFromVecAToVecB(const hmm_vec3 vec3From, const hmm_vec3 vec3To) {
    Rotor3 result;
    
    result.a = 1 + HMM_DotVec3(vec3To, vec3From);
    
    Bivec3 minusb = wedge(vec3To, vec3From);
    result.xy = minusb.xy;
    result.xz = minusb.xz;
    result.yz = minusb.yz;
    
    rotorNormalize(&result);
    
    return result;
}

ENGINE_API Rotor3
rotorFromAngleAndBivec(float angleRadian, Bivec3 bivec) {
	Rotor3 result;
    
    float sina = sinf(angleRadian / 2.f);
	result.a = cosf(angleRadian / 2.f);
    result.xy = -sina * bivec.xy;
	result.xz = -sina * bivec.xz;
	result.yz = -sina * bivec.yz;
    
    return result;
}

ENGINE_API inline Rotor3 
operator*(Rotor3 rotA, Rotor3 rotB) {
	Rotor3 result;
	
	result.a  = rotA.a * rotB.a - rotA.xy * rotB.xy - rotA.xz * rotB.xz - rotA.yz * rotB.yz;
	result.xy = rotA.xy * rotB.a + rotA.a * rotB.xy + rotA.yz * rotB.xz - rotA.xz * rotB.yz;
	result.xz = rotA.xz * rotB.a + rotA.a * rotB.xz - rotA.yz * rotB.xy + rotA.xy * rotB.yz;
	result.yz = rotA.yz * rotB.a + rotA.a * rotB.yz + rotA.xz * rotB.xy - rotA.xy * rotB.xz;
    
	return result;
}

ENGINE_API hmm_vec3
getRotatedVector(hmm_vec3 vec, Rotor3 rot) {
    hmm_vec3 q;
	q.x = rot.a * vec.x + vec.y * rot.xy + vec.z * rot.xz;
	q.y = rot.a * vec.y - vec.x * rot.xy + vec.z * rot.yz;
	q.z = rot.a * vec.z - vec.x * rot.xz - vec.y * rot.yz;
    
	float qxyz = - vec.x * rot.yz + vec.y * rot.xz - vec.z * rot.xy; // trivector
    
    hmm_vec3 result;
	result.x = rot.a * q.x + q.y * rot.xy + q.z * rot.xz - qxyz * rot.yz;
	result.y = rot.a * q.y - q.x * rot.xy + qxyz * rot.xz + q.z * rot.yz;
	result.z = rot.a * q.z - qxyz * rot.xy - q.x * rot.xz - q.y * rot.yz;
    
	return result;
}

ENGINE_API inline Rotor3 
&operator*=(Rotor3& rotA, Rotor3 rotB) {
    rotA = rotA * rotB;
	return rotA;
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
	return HMM_SQUARE(pRot->a) + HMM_SQUARE(pRot->xy) + 
        HMM_SQUARE(pRot->xz) + HMM_SQUARE(pRot->yz);
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
