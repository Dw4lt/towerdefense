#include <SDL/SDL.h>
#include <stdio.h>
#include"rendering/renderer_object.hpp"
#include"rendering/renderer.hpp"
#include"enemies/enemy.hpp"
#include"map/field.hpp"
#include"map/cursor.hpp"
#include<string>
#include<iostream>
#include <ctime>
#include <os.h>




int main ()
{
    srand(time(NULL));
    printf("############## Start ##############\n");

    Renderer* renderer = Renderer::Init(320, 240, 16);
    
    Field* field = new Field(0,0,320,210,40,30);
    Enemy* enemy = new Enemy(field->get(field->getStart())->getCenter(), 4, 4, Point{field->getStart()}, 100, 4.2, 0xf00f);
    Cursor* field_cursor = new Cursor(field);
    renderer->addToSchene(field,0);
    renderer->addToSchene(enemy, 2);
    renderer->addToSchene(field_cursor, 10);

    while (!isKeyPressed(KEY_NSPIRE_ESC)){
        Uint32 start = SDL_GetTicks();

        // Game Loop
        field_cursor->poll(); // Only if on the main field, separate cursor for shop
        enemy->pathfind(field);
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