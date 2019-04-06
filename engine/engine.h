#ifndef ENGINE_H
#define ENGINE_H
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "meow_intrinsics.h"
#include "meow_hash.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define HANDMADE_MATH_IMPLEMENTATION
#include "handmade_math.h"

#if OPENGL
#include "glad.c"
#include <glfw3.h>
#define NEAREST GL_NEAREST
#define LINEAR GL_LINEAR
#define CLAMP_TO_EDGE GL_CLAMP_TO_EDGE
#define REPEAT GL_REPEAT
#define MIRRORED_REPEAT GL_MIRRORED_REPEAT

#define KEY_W GLFW_KEY_W
#define KEY_S GLFW_KEY_S
#define KEY_A GLFW_KEY_A
#define KEY_D GLFW_KEY_D
#define KEY_Q GLFW_KEY_Q
#define KEY_E GLFW_KEY_E
#endif

#include "base.h"

#if ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include "camera.h"
#if OPENGL
#include "opengl_renderer.h"
#endif

#include "entities.h"
#include "physics.h"
#include "tilemap.h"

struct ENGINE_API Time {
    f32 deltaTime;
    f32 lastFrame;
};

global const hmm_vec3 VEC3_X = HMM_Vec3(1.f, 0.f, 0.f);
global const hmm_vec3 VEC3_Y = HMM_Vec3(0.f, 1.f, 0.f);
global const hmm_vec3 VEC3_Z = HMM_Vec3(0.f, 0.f, 1.f);

ENGINE_API void startEngine(Window* pWindow, Renderer* pRenderer, Camera* pCamera);
ENGINE_API void updateEngine(Window* pWindow, Time* pTime, CollisionManager* pCM = 0);
#endif //ENGINE_H