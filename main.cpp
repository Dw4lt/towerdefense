#include <stdio.h>
#include"enemies/enemy.hpp"
#include<string>

#include "game_manager.hpp"




int main ()
{
    srand(time(NULL));
    printf("############## Start ##############\n");
    GameManager manager;
    manager.start();
    printf("############## Stop ##############\n");
    return EXIT_SUCCESS;
}