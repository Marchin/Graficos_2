ENGINE_API void
updateProjection(Camera* pCamera) {
    switch (pCamera->projectionType) {
        case ORTHOGRAPHIC: {
            pCamera->projection = HMM_Orthographic(-pCamera->halfCamWidth, 
                                                   pCamera->halfCamWidth, 
                                                   -pCamera->halfCamHeight, 
                                                   pCamera->halfCamHeight, 
                                                   0.f, 100.f);
        }break;
        case PERSPECTIVE: {
            pCamera->projection = HMM_Perspective(pCamera->fov,
                                                  pCamera->aspectRatio,
                                                  0.1f, 100.f); 
        }break;
        default: {
            assert(0);
        }break;
    }
}

ENGINE_API void 
initCamera(Camera* pCamera, hmm_vec3 position, hmm_vec3 up, f32 yaw, f32 pitch) {
    
    pCamera->front = HMM_Vec3(0.0f, 0.0f, -1.0f);
	pCamera->position = position;
	pCamera->up = up;
	pCamera->worldUp = HMM_Vec3(0.f, 1.f, 0.f);
	pCamera->yaw = yaw;
	pCamera->pitch = pitch;
    pCamera->movementSpeed = SPEED;
    pCamera->mouseSensitivity = SENSITIVITY;
    pCamera->zoom = ZOOM;
    pCamera->halfCamHeight = 10.f; 
    pCamera->halfCamWidth = 10.f; 
    pCamera->fov = 45;
    pCamera->aspectRatio = 800/600;
    pCamera->projectionType = PERSPECTIVE;
    pCamera->model = HMM_Mat4d(1.f);
    pCamera->firstMouseMovement = true;
    updateCameraVectors(pCamera);
    updateProjection(pCamera);
}

ENGINE_API void
intiCamera(Camera* pCamera,
           f32 posX, f32 posY, f32 posZ, 
           f32 upX, f32 upY, f32 upZ, 
           f32 yaw, f32 pitch) {
    
    pCamera->front = HMM_Vec3(0.0f, 0.0f, -1.0f);
    pCamera->position = HMM_Vec3(posX, posY, posZ);
    pCamera->worldUp = HMM_Vec3(upX, upY, upZ);
    pCamera->yaw = yaw;
    pCamera->pitch = pitch;
    pCamera->movementSpeed = SPEED;
    pCamera->mouseSensitivity = SENSITIVITY;
    pCamera->zoom = ZOOM;
    pCamera->firstMouseMovement = true;
    updateCameraVectors(pCamera);
}

ENGINE_API hmm_mat4 
getViewMatrix(Camera* pCamera) {
    return HMM_LookAt(pCamera->position, pCamera->position + pCamera->front, pCamera->up);
}

ENGINE_API void 
moveCamera(Camera* pCamera, hmm_vec3 direction, f32 deltaTime) {
    f32 velocity = pCamera->movementSpeed * deltaTime;
    pCamera->position += direction * velocity;
}

ENGINE_API void
cameraMouseMovement(Camera* pCamera, f64 xPos, f64 yPos, b32 constrainPitch) {
    if (pCamera->firstMouseMovement) {
        pCamera->lastMousePos.x = (f32)xPos;
        pCamera->lastMousePos.y = (f32)yPos;
        pCamera->firstMouseMovement = false;
    }
    
    f32 xOffset = (f32)xPos - pCamera->lastMousePos.x;
    f32 yOffset = pCamera->lastMousePos.y - (f32)yPos; // reversed since y-coordinates go from bottom to top
    pCamera->lastMousePos.x = (f32)xPos;
    pCamera->lastMousePos.y = (f32)yPos;
    
    xOffset *= pCamera->mouseSensitivity;
    yOffset *= pCamera->mouseSensitivity;
    
    pCamera->yaw += xOffset;
    pCamera->pitch += yOffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pCamera->pitch > 89.0f) {
            pCamera->pitch = 89.0f;
        } if (pCamera->pitch < -89.0f) {
            pCamera->pitch = -89.0f;
        }
    }
    
    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors(pCamera);
}

ENGINE_API void
cameraMouseScroll(Camera* pCamera, f32 yoffset) {
    if (pCamera->zoom >= 1.0f && pCamera->zoom <= 45.0f) {
        pCamera->zoom -= yoffset;
    }
    if (pCamera->zoom <= 1.0f) {
        pCamera->zoom = 1.0f;
    }
    if (pCamera->zoom >= 45.0f) {
        pCamera->zoom = 45.0f;
    }
}

ENGINE_API void
updateCameraVectors(Camera* pCamera) {
    // Calculate the new Front vector
    hmm_vec3 front;
    front.x = cosf(HMM_ToRadians((f32)pCamera->yaw))*cosf(HMM_ToRadians((f32)pCamera->pitch));
    front.y = sinf(HMM_ToRadians((f32)pCamera->pitch));
    front.z = sinf(HMM_ToRadians((f32)pCamera->yaw))*cosf(HMM_ToRadians((f32)pCamera->pitch));
    
    hmm_vec3 up;
    up.x = sinf(HMM_ToRadians((f32)pCamera->roll));
    up.y = cosf(HMM_ToRadians((f32)pCamera->pitch))*cosf(HMM_ToRadians((f32)pCamera->roll));
    up.z = sinf(HMM_ToRadians((f32)pCamera->pitch))*cosf(HMM_ToRadians((f32)pCamera->roll));
    pCamera->front = HMM_NormalizeVec3(front);
    pCamera->up = HMM_NormalizeVec3(up);
    // Also re-calculate the Right and Up vector
    pCamera->right = HMM_NormalizeVec3(HMM_Cross(pCamera->front, pCamera->up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    pCamera->up = HMM_NormalizeVec3(HMM_Cross(pCamera->right, pCamera->front));
}
