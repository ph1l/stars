//Include SDL functions and datatypes
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "starslib.h"

#include "SDL/SDL.h"

#define WIDTH 1024
#define HEIGHT 768

#define MAX_Z 512
#define FRAME_DELAY 2

//Translate XY coordinates with origin at the center of the screen to framebuffer pixel number
#define LOC( x, y ) (((WIDTH)*(( (y*-1)+(HEIGHT/2) )-1))+(( x+(WIDTH/2) )-1))

int main( int argc, char* args[] )
{

	//The images
	SDL_Surface* screen = NULL;
	SDL_Event event;

	//Start SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	//Set up screen
	screen = SDL_SetVideoMode (WIDTH, HEIGHT, 32, 0);

	if(screen == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	} 

        // Initialize the stars lib
	struct universe* u;
	new_universe( &u, WIDTH, HEIGHT, MAX_Z );

	int frames = 0;
	int quit = 0;

	while (!quit) {


		// Blank the screen buffer
		SDL_FillRect(screen, NULL, 0x000000);

		// Process/Draw all the things
		for (;;) {
			struct return_point rp;
			int return_code = process_point( u, &rp  );
			if (return_code == 0) { break; }
			if (return_code == 1) {
				Uint32 p = SDL_MapRGB(
					screen->format,
					255*rp.opacity/OPACITY_MAX,
					255*rp.opacity/OPACITY_MAX,
					255*rp.opacity/OPACITY_MAX
				);
				( (Uint32 *) screen->pixels)[LOC(rp.x, rp.y)] = p;

			}
		}
		frames++;

		//Update Screen
		SDL_Flip( screen );

		for ( int c=0; c< rand()%128;c++){
			new_point( u );
		}

		//Pause
		SDL_Delay( FRAME_DELAY );

		while(SDL_PollEvent(&event))
        	{
			if (event.type == SDL_QUIT)
				quit = 1;
		}
		//Get the keystates
		Uint8 *keystates = SDL_GetKeyState( NULL ); 
		if( keystates[ SDLK_q ] )
		{
			quit = 1;
		}
		
	}

	//Quit SDL
	SDL_Quit();

	return 0;
}
