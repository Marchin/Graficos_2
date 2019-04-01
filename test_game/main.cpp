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
    
    startEngine(&window, &renderer);
    initGame(&game, &renderer, &time, &cm);
    while (!windowShouldClose(&window)) {
        updateGame(&game, &renderer, &time, &cm);
        updateEngine(&window, &time, &cm);
    }
    stopRenderer();
    stopWindow(&window);
}

