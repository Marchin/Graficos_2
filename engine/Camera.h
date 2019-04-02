#ifndef CAMERA_H
#define CAMERA_H

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const f32 YAW = -90.0f;
const f32 PITCH = 0.0f;
const f32 SPEED = 2.5f;
const f32 SENSITIVITY = 0.1f;
const f32 ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
struct ENGINE_API Camera {
	// Camera Attributes
	hmm_vec3 position;
	hmm_vec3 front;
	hmm_vec3 up;
	hmm_vec3 right;
	hmm_vec3 worldUp;
	// Euler Angles
	f32 yaw;
	f32 pitch;
	// Camera options
	f32 movementSpeed;
	f32 mouseSensitivity;
	f32 zoom;
    
	// Constructor with vectors
	Camera(hmm_vec3 position = hmm_vec3(0.0f, 0.0f, 0.0f), hmm_vec3 up = hmm_vec3(0.0f, 1.0f, 0.0f),
           f32 yaw = YAW, f32 pitch = PITCH);
	
	// Constructor with scalar values
	Camera(f32 posX, f32 posY, f32 posZ, f32 upX, f32 upY, f32 upZ, f32 yaw, f32 pitch);
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	hmm_mat4 GetViewMatrix();
    
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, f32 deltaTime);
    
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(f32 xoffset, f32 yoffset, GLboolean constrainPitch = true);
    
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(f32 yoffset);
    
    private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

#endif //CAMERA_H