#include "engine.h"

#if OPENGL
#include "opengl_renderer.cpp"
#endif

#include "rotor.cpp"
#include "entities.cpp"
#include "physics.cpp"
#include "tilemap.cpp"
#include "camera.cpp"

ENGINE_API void
startEngine(Window* pWindow, Renderer* pRenderer, Camera* pCamera) {
    startWindow(pWindow);
    startRenderer(pRenderer, pWindow, pCamera);
}

ENGINE_API void
updateEngine(Window* pWindow, Time* pTime, CollisionManager* pCM) {
    swapBuffers(pWindow);
    pollEventsFromWindow(pWindow);
    fillColor(0.1f, 0.1f, 0.1f);
    clearRenderer();
    f32 currentFrame = getTime();
    pTime->deltaTime = currentFrame - pTime->lastFrame;
    pTime->lastFrame = currentFrame;
    if (pCM) {
        updateCollision(pCM);
    }
}
