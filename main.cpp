#include <SDL/SDL.h>
#include <stdio.h>
#include"rendering/renderer_object.hpp"
#include"rendering/renderer.hpp"
#include"enemies/enemy.hpp"
#include"map/field.hpp"
#include<string>
#include<iostream>
#include <ctime>




int main ()
{
    srand(time(NULL));
    printf("############## Start ##############\n");

    Renderer* renderer = Renderer::Init(320, 240, 16);
    
    Field* field = new Field(0,0,320,210,40,30);
    Enemy* enemy = new Enemy(field->get(field->getStart())->getCenter(), 4, 4, Point{field->getStart()}, 100, 4.2, 0xf00f);
    renderer->addToSchene(field,0);
    renderer->addToSchene(enemy, 2);

    for (int i = 0; i < 240; i++){
        enemy->pathfind(field);
        Uint32 start = SDL_GetTicks();
        renderer->render();
        renderer->display();
        Uint32 stop = SDL_GetTicks();
        Uint32 tick_duration = stop - start;
        if (tick_duration < 41){
            SDL_Delay(41-tick_duration);
        }
    }
    //SDL_UpdateRect(screen,0,0,50,50);
    //if (SDL_Flip(screen) != 0){
    //    SDL_SetError("WOOPS");
    //}
    // Wait for 5 sec
    //SDL_Delay( 5000 );
    delete renderer;
    printf("Stop.");
    return EXIT_SUCCESS;
}