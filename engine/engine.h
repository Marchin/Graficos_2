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
#endif

#include "base.h"

#if ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

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

ENGINE_API void startEngine(Window* pWindow, Renderer* pRenderer);
ENGINE_API void updateEngine(Window* pWindow, Time* pTime, CollisionManager* pCM = 0);
#endif //ENGINE_H