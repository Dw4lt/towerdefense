#include <SDL/SDL.h>
#include <stdio.h>

void set_pixel(SDL_Surface *surface, int x, int y, Uint16 pixel)
{
  Uint16 *target_pixel = (Uint16 *) surface->pixels + y * surface->pitch +
                                                     x * sizeof *target_pixel;
  *target_pixel = pixel;
}


int main ()
{
    printf("Now this is speed debugging.");
    SDL_WM_SetCaption("My Game Window", "game");
    SDL_Surface *screen = SDL_SetVideoMode(320, 240, 16, SDL_FULLSCREEN);
    //SDL_SetError("pixels: %i\npitch:%i",screen->pixels, screen->pitch);
    for (int i=0; i<50; i++){
        set_pixel(screen, i,i, 0xF800);
    }
    SDL_UpdateRect(screen,0,0,50,50);
    //if (SDL_Flip(screen) != 0){
    //    SDL_SetError("WOOPS");
    //}
    // Wait for 5 sec
    SDL_Delay( 5000 );


    SDL_free(screen);
    return EXIT_SUCCESS;
}