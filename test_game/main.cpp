#include <engine.h>
#include "game_entities.h"
#include "game.h"
#include "game_entities.cpp"
#include "game.cpp"

void main() {
    Window window = {};
    Renderer renderer = {};
    Time time = {};
    CollisionManager cm = {};
    Game game = {};
    window.pName = "Marmot";
    window.height = 600;
    window.width = 800;
    startEngine(&window, &renderer, &game.camera);
    initGame(&game, &renderer, &time, &cm);
    while (!windowShouldClose(&window)) {
        enginePreUpdate(&window, &time, &cm);
        updateGame(&game, &renderer, &time, &cm);
        enginePostUpdate(&window, &time, &cm);
    }
    s32 err = Pa_Terminate();
    if (err != paNoError){
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    stopRenderer();
    stopWindow(&window);
}

