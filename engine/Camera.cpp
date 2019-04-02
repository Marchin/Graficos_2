ENGINE_DLL void
initCamera(hmm_vec3 position, hmm_vec3 up, f32 yaw, f32 pitch) :
Front(hmm_vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

ENGINE_DLL void
intiCamera(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 yaw, f32 pitch) :
Front(hmm_vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    
	Position = hmm_vec3(posX, posY, posZ);
	WorldUp = hmm_vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

ENGINE_DLL hmm_mat4 
cameraGetViewMatrix() {
	return HMM_LookAt(Position, Position + Front, Up);
}

ENGINE_DLL void 
cameraProcessKeyboard(Camera_Movement direction, f32 deltaTime) {
	f32 velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

ENGINE_DLL void
cameraMouseMovement(f32 xoffset, f32 yoffset, GLboolean constrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;
    
	Yaw += xoffset;
	Pitch += yoffset;
    
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
    
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

ENGINE_DLL void
cameraMouseScroll(f32 yoffset) {
	if (Zoom >= 1.0f && Zoom <= 45.0f) {
		Zoom -= yoffset;
    }
	if (Zoom <= 1.0f) {
		Zoom = 1.0f;
    }
	if (Zoom >= 45.0f) {
		Zoom = 45.0f;
    }
}

ENGINE_DLL void
updateCameraVectors() {
	// Calculate the new Front vector
	hmm_vec3 front;
	front.x = cos(HMM_ToRadians(Yaw)) * cos(HMM_ToRadians(Pitch));
	front.y = sin(HMM_ToRadians(Pitch));
	front.z = sin(HMM_ToRadians(Yaw)) * cos(HMM_ToRadians(Pitch));
	Front = HMM_NormalizeVec3(front);
	// Also re-calculate the Right and Up vector
	Right = HMM_NormalizeVec3(HMM_Cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = HMM_NormalizeVec3(HMM_Cross(Right, Front));
}
