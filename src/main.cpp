#include "enemies/enemy.hpp"
#include <stdio.h>
#include <string>
#include <time.h>

#include "game_manager.hpp"

int main() {
    srand(time(NULL));
    printf("############## Start ##############\n");
    GameManager manager;
    manager.start();
    printf("############## Stop ##############\n");
    return EXIT_SUCCESS;
}