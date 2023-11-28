#include "enemies/enemy.hpp"
#include <ctime>
#include <SDL/SDL.h>

#include "game_manager.hpp"
#include "util/macros.hpp"
#include "state_machines/main_sm.hpp"

int main() {
    srand(time(NULL));
    LOG("Start\n");
    SDL_Init( SDL_INIT_VIDEO );

    try {
        auto manager = ROwner<GameManager>(new GameManager());

        auto main_sm = MainSM(manager.makeReader());

        main_sm.start();
        while (!main_sm.stopped()){
            main_sm.tick();
        }
    } catch (char const* e) {
        LOG("%s\n", e);
    }
    SDL_Quit();
    LOG("Exited gracefully\n");
    return EXIT_SUCCESS;
}
