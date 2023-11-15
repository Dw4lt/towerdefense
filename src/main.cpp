#include "enemies/enemy.hpp"
#include <stdio.h>
#include <string>
#include <time.h>
#include <SDL/SDL.h>

#include "game_manager.hpp"

int main() {
    srand(time(NULL));
    printf("############## Start ##############\n");
    SDL_Init( SDL_INIT_VIDEO );
    try {
        GameManager manager;
        manager.start();
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    } catch (char const* e) {
        std::cout << e << "\n";
    }
    SDL_Quit();
    printf("############## Stop ##############\n");
    return EXIT_SUCCESS;
}
