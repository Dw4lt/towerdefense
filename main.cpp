#include <SDL/SDL.h>
#include <stdio.h>
#include"rendering/renderer_object.hpp"
#include"rendering/renderer.hpp"
#include"enemies/enemy.hpp"
#include"map/field.hpp"




int main ()
{
    printf("Start.");

    Renderer* renderer = Renderer::Init(320, 240, 16);
    
    Enemy* enemy = new Enemy(15, 30, 15, 15, 100, 0xf00f);
    renderer->addToSchene(enemy, 2);
    Field* field = new Field(0,0,320,240,12,10);
    renderer->addToSchene(field,0);


    renderer->render();
    renderer->display();
    //SDL_SetError("pixels: %i\npitch:%i",screen->pixels, screen->pitch);
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