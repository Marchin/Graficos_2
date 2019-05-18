#include "engine.h"

#if OPENGL
#include "opengl_renderer.cpp"
#endif

#include "entities.cpp"
#include "physics.cpp"
#include "tilemap.cpp"
#include "camera.cpp"

ENGINE_API void
startEngine(Window* pWindow, Renderer* pRenderer, Camera* pCamera) {
    gpMeshComponentsPool = (MeshComponentsPool*)malloc(sizeof(MeshComponentsPool));
    assert(gpMeshComponentsPool);
    memset(gpMeshComponentsPool, 0, sizeof(MeshComponentsPool));
    
    startWindow(pWindow);
    startRenderer(pRenderer, pWindow, pCamera);
}

ENGINE_API void
enginePreUpdate(Window* pWindow, Time* pTime, CollisionManager* pCM) {
    //swapBuffers(pWindow);
    //pollEventsFromWindow(pWindow);
    fillColor(.2f, .2f, .2f);
    clearRenderer();
    f32 currentFrame = getTime();
    pTime->deltaTime = currentFrame - pTime->lastFrame;
    pTime->lastFrame = currentFrame;
}

ENGINE_API void
enginePostUpdate(Window* pWindow, Time* pTime, CollisionManager* pCM) {
    swapBuffers(pWindow);
    pollEventsFromWindow(pWindow);
    if (pCM) {
        updateCollision(pCM);
    }
}