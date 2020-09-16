inline Bivec3
wedge(const hmm_vec3 vec3A, const hmm_vec3 vec3B) {
    Bivec3 result = {
        vec3A.y*vec3B.z - vec3A.z*vec3B.y,
        vec3A.z*vec3B.x - vec3A.x*vec3B.z,
        vec3A.x*vec3B.y - vec3A.y*vec3B.x,
    };
	return result;
}

inline f32
bivecLengthSqr(Bivec3 bivec) {
	return bivec.yz*bivec.yz + bivec.zx*bivec.zx + bivec.xy*bivec.xy;
}

inline f32
bivecLength(Bivec3 bivec) {
	return sqrtf(bivecLengthSqr(bivec));
}

inline Bivec3
bivecNormalize(Bivec3 bivec) {
	f32 length = bivecLength(bivec);
	bivec.yz /= length; bivec.zx /= length; bivec.xy /= length;
    
    return bivec;
}

Rotor3
rotorFromVecAToVecB(const hmm_vec3 from, const hmm_vec3 to) {
    Rotor3 result;
    
    result.a = HMM_DotVec3(from, to);
    result.bivec = wedge(from, to);
    
    return result;
}

Rotor3
rotorFromAngleAndAxis(f32 angleRadian, hmm_vec3 axis) {
	Rotor3 result;
    
    f32 axisNorm = 0;
    f32 halfAngle = angleRadian*0.5f;
    f32 sinAngle = sinf(halfAngle);
    axisNorm = HMM_SquareRootF(HMM_DotVec3(axis, axis));
    
    result.a = cosf(angleRadian);
    result.yz = axis.x * sinAngle / axisNorm;
    result.zx = axis.y * sinAngle / axisNorm;
    result.xy = axis.z * sinAngle / axisNorm;
    
    return result;
}

Rotor3
rotorFromAngleAndBivec(Bivec3 bivec, f32 angleRadian) {
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


inline Rotor3
rotateYZ(f32 angle) {
    f32 halfAngle = angle*0.5f;
    f32 sinAngle = sinf(halfAngle);
    f32 cosAngle = cosf(halfAngle);
    
    Rotor3 result = {};
    
    result.a = cosAngle;
    result.yz = sinAngle;
    
    return result;
}

inline Rotor3
rotateZX(f32 angle) {
    f32 halfAngle = angle*0.5f;
    f32 sinAngle = sinf(halfAngle);
    f32 cosAngle = cosf(halfAngle);
    
    Rotor3 result = {};
    
    result.a = cosAngle;
    result.zx = sinAngle;
    
    return result;
}

inline Rotor3
rotateXY(f32 angle) {
    f32 halfAngle = angle*0.5f;
    f32 sinAngle = sinf(halfAngle);
    f32 cosAngle = cosf(halfAngle);
    
    Rotor3 result = {};
    
    result.a = cosAngle;
    result.xy = sinAngle;
    
    return result;
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

hmm_vec3
getRotatedVectorX(Rotor3 rot) {
    //p = rot*vec
    hmm_vec3 p;
    
    p.x = rot.a;
    p.y = -rot.xy;
    p.z = rot.zx;
    
    f32 xyz = rot.yz;
    
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
rotorLengthSqr(Rotor3 rot) {
	return rot.a*rot.a + rot.yz*rot.yz + rot.zx*rot.zx + rot.xy*rot.xy;
}

inline f32
rotorLength(Rotor3 rot) {
	return sqrtf(rotorLengthSqr(rot));
}

inline Rotor3
rotorNormalize(Rotor3 rot) {
	f32 length = rotorLength(rot);
	rot.a /= length; rot.yz /= length; rot.zx /= length; rot.xy /= length;
    
    return rot;
}

inline hmm_vec3
getRotorNormal(Rotor3 rot) {
    hmm_vec3 result = {};
    
    result.x = rot.yz;
    result.y = rot.zx;
    result.z = rot.xy;
    
    return result;
}

inline hmm_mat4
getRotorMat4(hmm_vec3 Eye, hmm_vec3 Center, hmm_vec3 Up) {
    /*
        hmm_vec3 vecX = getRotatedVector(VEC3_X, rot);
        hmm_vec3 vecY = getRotatedVector(VEC3_Y, rot);
        hmm_vec3 vecZ = getRotatedVector(VEC3_Z, rot);
        
        hmm_mat4 mat;
        mat.Rows[0] = { vecX[0], vecX[1], vecX[2], 0.f };
        mat.Rows[1] = { vecY[0], vecY[1], vecY[2], 0.f };
        mat.Rows[2] = { vecZ[0], vecZ[1], vecZ[2], 0.f };
        mat.Rows[3] = { 0.f, 0.f, 0.f, 1.f };
         */
    
    hmm_mat4 Result;
    
    hmm_vec3 F = HMM_NormalizeVec3(HMM_SubtractVec3(Center, Eye));
    Bivec3 S = bivecNormalize(wedge(F, Up));
    Bivec3 U = wedge(S.vec, F);
    hmm_vec3 a = HMM_Cross(S.vec, F);
    
    Result.Elements[0][0] = S.yz;
    Result.Elements[0][1] = U.yz;
    Result.Elements[0][2] = -F.X;
    Result.Elements[0][3] = 0.0f;
    
    Result.Elements[1][0] = S.zx;
    Result.Elements[1][1] = U.zx;
    Result.Elements[1][2] = -F.Y;
    Result.Elements[1][3] = 0.0f;
    
    Result.Elements[2][0] = S.xy;
    Result.Elements[2][1] = U.xy;
    Result.Elements[2][2] = -F.Z;
    Result.Elements[2][3] = 0.0f;
    
    Result.Elements[3][0] = -HMM_DotVec3(S.vec, Eye);
    Result.Elements[3][1] = -HMM_DotVec3(U.vec, Eye);
    Result.Elements[3][2] = HMM_DotVec3(F, Eye);
    Result.Elements[3][3] = 1.0f;
    
    return (Result);
}