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
    axis = HMM_NormalizeVec3(axis);
    
    result.a = cosf(halfAngle);
    result.yz = (axis.x * sinAngle);
    result.zx = (axis.y * sinAngle);
    result.xy = (axis.z * sinAngle);
    
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
    
    result.a = a.a*b.a - a.yz*b.yz - a.zx*b.zx - a.xy*b.xy;
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


V3
getRotatedVector(V3 vec, Rotor3 rot) {
#if 0
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
#else
    // TODO(Marchin): Expando and simplify
    f32 l = rot.yz*vec.x + rot.zx*vec.y + rot.xy*vec.z;
    f32 i = rot.a*vec.x + rot.xy*vec.y - rot.zx*vec.z;
    f32 j = -rot.xy*vec.x + rot.a*vec.y + rot.yz*vec.z;
    f32 k = rot.zx*vec.x - rot.yz*vec.y + rot.a*vec.z;
    
    V3 result;
    result.x = rot.yz*l + rot.a*i + rot.xy*j - rot.zx*k;
    result.y = rot.zx*l - rot.xy*i + rot.a*j + rot.yz*k;
    result.z = rot.xy*l + rot.zx*i - rot.yz*j + rot.a*k;
    
    return result;
#endif
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
    Rotor3 result = rotor * rotated;
    Rotor3 reversed = rotorGetReverse(rotor);
    result = result * reversed;
	return result;
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

inline Rotor3
lookAt(V3 pos, V3 target) {
    V3 dir = HMM_NormalizeVec3(target - pos);
    Rotor3 rotor;
    
    if (dir.z != 0.0f) {
        rotor.yz = cosf(acosf(dir.y/dir.z)*0.5f);
    } else {
        rotor.yz = cosf(45.f);
    }
    
    if (dir.x != 0.0f) {
        rotor.zx = cosf(acosf(dir.z/dir.x)*0.5f);
    } else {
        rotor.zx = cosf(45.f);
    }
    
    if (dir.y != 0.0f) {
        rotor.xy = cosf(acosf(dir.x/dir.y)*0.5f);
    } else {
        rotor.xy = cosf(45.f);
    }
    
    rotor = rotorNormalize(rotor);
    
    return rotor;
}

inline Mat4
getRotorMat4(Rotor3 rotor) {
    Mat4 result;
    
    if (rotor.a != 1.f) {
        s32 a = 3;
    }
    
    f32 xx2 = 2*rotor.dx*rotor.dx;
    f32 yy2 = 2*rotor.dy*rotor.dy;
    f32 zz2 = 2*rotor.dz*rotor.dz;
    f32 xy2 = 2*rotor.dx*rotor.dy;
    f32 yz2 = 2*rotor.dy*rotor.dz;
    f32 zx2 = 2*rotor.dz*rotor.dx;
    f32 ax2 = 2*rotor.a*rotor.dx;
    f32 ay2 = 2*rotor.a*rotor.dy;
    f32 az2 = 2*rotor.a*rotor.dz;
    result.Rows[0] = { 1 - yy2 - zz2, xy2 + az2, zx2 - ay2, 0.f };
    result.Rows[1] = { xy2 - az2, 1 - zz2 - xx2, yz2 + ax2, 0.f };
    result.Rows[2] = { zx2 + ay2, yz2 - ax2, 1 - xx2 - yy2, 0.f };
    result.Rows[3] = { 0.f, 0.f, 0.f, 1.f };
    
    return result;
}