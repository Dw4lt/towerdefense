#include <SDL/SDL.h>
#include <stdio.h>
#include"rendering/renderer_object.hpp"
#include"rendering/renderer.hpp"
#include"enemies/enemy.hpp"




int main ()
{
    printf("Start.");

    Renderer* renderer = Renderer::Init(320, 240, 16);
    Enemy* enemy = new Enemy(15, 30, 15, 15, 100, 0xf00f);
    renderer->addToSchene(enemy, 1);
    renderer->render();
    renderer->display();
    //SDL_SetError("pixels: %i\npitch:%i",screen->pixels, screen->pitch);
    //SDL_UpdateRect(screen,0,0,50,50);
    //if (SDL_Flip(screen) != 0){
    //    SDL_SetError("WOOPS");
    //}
    // Wait for 5 sec
    SDL_Delay( 5000 );
    printf("Stop.");
    return EXIT_SUCCESS;
}