//Include SDL functions and datatypes
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "stars.h"

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

	//Initialize points list
	struct points *head_point_ptr = get_new_point();
	struct points *p_ptr, *last_ptr = NULL;

	int quit = 0;
	while (!quit) {

		// Blank the screen buffer
		SDL_FillRect(screen, NULL, 0x000000);

		// DO SOME SHIT
		p_ptr = head_point_ptr;
		last_ptr = NULL;
		// int count = 0;
		while (p_ptr != NULL){
			// printf("p_ptr: x, y, z == %i %li: %i, %i, %i\n", count++, (long int) p_ptr, p_ptr->x, p_ptr->y, p_ptr->z);
			if ( p_ptr->z == 0 ){
				// Delete point that has reached us.
				if ( last_ptr == NULL ) {
					head_point_ptr = p_ptr->next;
				} else {
					last_ptr->next = p_ptr->next;
				}
				p_ptr = p_ptr->next;
				free(p_ptr);
				continue;
			} else {
				// Draw the point
				int x, y;
				x = p_ptr->x / p_ptr->z;
				y = p_ptr->y / p_ptr->z;
				if ( abs(x) >= WIDTH/2 || abs(y) >= HEIGHT/2 ){
					// Delete point that is off screen
					if ( last_ptr == NULL ) {
						head_point_ptr = p_ptr->next;
						last_ptr = p_ptr;
						p_ptr = p_ptr->next;
						free(last_ptr);
						last_ptr = NULL;
					} else {
						last_ptr->next = p_ptr->next;
						free(p_ptr);
						p_ptr = last_ptr->next;
					}
					continue;
				} else {
					int m = 255*((MAX_Z-p_ptr->z)*2)/MAX_Z;
					if ( m>255 ){ m=255; }
					Uint32 p = SDL_MapRGB(
							screen->format,
							p_ptr->r*m/255,
							p_ptr->g*m/255,
							p_ptr->b*m/255
						);

					( (Uint32 *) screen->pixels)[LOC(x, y)] = p;
					p_ptr->z = p_ptr->z - 1;
					last_ptr = p_ptr;
					p_ptr = p_ptr->next;
				}
			}
		}

		for ( int c=0; c< rand()%128;c++){
			last_ptr->next = get_new_point();
			last_ptr = last_ptr->next;
		}

		//Update Screen
		SDL_Flip( screen );

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

struct points* get_new_point()
{

	struct points* p_ptr = malloc(sizeof(struct points));

	p_ptr->x    = (rand()%WIDTH - (WIDTH/2)) * MAX_Z;
	p_ptr->y    = (rand()%HEIGHT - (HEIGHT/2)) * MAX_Z;
	p_ptr->z    = MAX_Z;

	p_ptr->r = rand()%256;
	p_ptr->g = rand()%256;
	p_ptr->b = rand()%256;

	p_ptr->next = NULL;

	return p_ptr;
}
