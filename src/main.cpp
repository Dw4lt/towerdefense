#include "enemies/enemy.hpp"
#include <stdio.h>
#include <string>
#include <time.h>
#include <SDL/SDL.h>

#include "game_manager.hpp"
#include "primitives/util.hpp"

int main() {
    srand(time(NULL));
    LOG("Start\n");
    SDL_Init( SDL_INIT_VIDEO );

    try {
        GameManager manager;
        manager.start();
    } catch (std::exception& e) {
        LOG("%s\n", e.what());
    } catch (char const* e) {
        LOG("%s\n", e);
    }
    SDL_Quit();
    LOG("Exited gracefully\n");
    return EXIT_SUCCESS;
}
