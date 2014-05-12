//Include SDL functions and datatypes
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "SDL/SDL.h"

#define WIDTH 1024
#define HEIGHT 768

#define MAX_Z 512
#define FRAME_DELAY 2

//Translate XY coordinates with origin at the center of the screen to framebuffer pixel number
#define LOC( x, y ) (((WIDTH)*(( (y*-1)+(HEIGHT/2) )-1))+(( x+(WIDTH/2) )-1))

struct points
{
	int x, y, z;
	Uint8 r, g, b;
	struct points *next;
};

void new_point( struct points** head )
{

	struct points* p_ptr = malloc(sizeof(struct points));

	p_ptr->x    = (rand()%WIDTH - (WIDTH/2)) * MAX_Z;
	p_ptr->y    = (rand()%HEIGHT - (HEIGHT/2)) * MAX_Z;
	p_ptr->z    = MAX_Z;

	p_ptr->r = rand()%256;
	p_ptr->g = rand()%256;
	p_ptr->b = rand()%256;

	p_ptr->next = *head;
	*head = p_ptr;
	#ifdef DEBUG
	printf("NEW POINT: %lx: (%i,%i,%i) (%i,%i,%i) next=%lx\n", (long unsigned int )p_ptr, p_ptr->x, p_ptr->y, p_ptr->z, p_ptr->r, p_ptr->g, p_ptr->b, (long unsigned int) p_ptr->next);
	#endif

	return;
}

void kill_point( struct points** head, struct points* to_kill )
{

	struct points *p_ptr, *last_ptr = NULL;


	for ( p_ptr = *head; p_ptr != NULL; p_ptr = p_ptr->next)
	{
		if (p_ptr == to_kill)
		{
			#ifdef DEBUG
			printf("KILL POINT: %lx: (%i,%i,%i) (%i,%i,%i).\n", (long unsigned int )p_ptr, p_ptr->x, p_ptr->y, p_ptr->z, p_ptr->r, p_ptr->g, p_ptr->b);
			#endif
			if (last_ptr == NULL)
			{
				*head = p_ptr->next;
			} else {
				last_ptr-> next = p_ptr->next;
			}
			free(p_ptr);
		} else {
			last_ptr = p_ptr;
		}
	}
	return;
}


int process_point( SDL_Surface *screen, struct points *p_ptr )
{
	int count = 0;

	if ( p_ptr->z == 0 ){
		// Delete point that has reached us.
		return(0);
	} else {
		// Plot the point
		int x, y;
		x = p_ptr->x / p_ptr->z;
		y = p_ptr->y / p_ptr->z;
		if ( abs(x) >= WIDTH/2 || abs(y) >= HEIGHT/2 ){
			// Delete point that is off screen
			return(0);
		} else {
			count++;
			int m = 255*((MAX_Z-p_ptr->z)*4)/MAX_Z;
			if ( m>255 ){ m=255; }
			Uint32 p = SDL_MapRGB(
					screen->format,
					p_ptr->r*m/255,
					p_ptr->g*m/255,
					p_ptr->b*m/255
				);
			( (Uint32 *) screen->pixels)[LOC(x, y)] = p;
			p_ptr->z = p_ptr->z - 1;
			p_ptr->x = p_ptr->x;
			p_ptr->y = p_ptr->y;
			return(1);
		}
	}
}


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

	// time_t start_t = time(NULL);
	// time_t cur_t = NULL;

	// Initialize Head List Pointer
	struct points *head_point_ptr = NULL;
	new_point( &head_point_ptr );

	struct points *p_ptr, *next_ptr = NULL;

	int frames = 0;
	int quit = 0;
	while (!quit) {


		// Blank the screen buffer
		SDL_FillRect(screen, NULL, 0x000000);

		// DO SOME SHIT
		p_ptr = head_point_ptr;
		//int count = 0;
		while (p_ptr != NULL){
			next_ptr = p_ptr->next;
			//printf("advance_animation: x, y, z == %i %li: %i, %i, %i", count++, (long int) p_ptr, p_ptr->x, p_ptr->y, p_ptr->z);
			if ( process_point(screen, p_ptr) == 0 )
			{
				kill_point( &head_point_ptr, p_ptr );
			}
			//printf(".\n");
			p_ptr = next_ptr;
		}
		frames++;
		//cur_t = time(NULL);
		//if (cur_t > start_t)
		//{
		//	printf("fps, points: = %i, %i\n", (int) frames/(cur_t-start_t), count);
		//}

		for ( int c=0; c< rand()%128;c++){
			new_point(&head_point_ptr);
		}

		//Update Screen
		SDL_Flip( screen );

		//Pause
		//SDL_Delay( FRAME_DELAY );

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
