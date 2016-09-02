/*
 *    Copyright 2016 Philip J Freeman <elektron@halo.nu>
 *
 *    This file is part of stars.
 *
 *    stars is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    stars is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with stars.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <X11/Xlib.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "starslib.h"

#define Z_START 1024
#define U_SLEEP 25000
#define SPAWN_PERCENT 25
#define POSSIBLE_SPAWNS 1

int main()
{
	// Open the display

	Display *dpy = XOpenDisplay(NULL);
	assert(dpy);

	// Get the root window and attributes

	Window w = DefaultRootWindow(dpy);

	XWindowAttributes w_attr;
	assert(XGetWindowAttributes(dpy, w, &w_attr));

	// Create a "Graphics Context"

	Colormap colormap;
	colormap = DefaultColormap(dpy, DefaultScreen(dpy));

	GC gc[OPACITY_MAX];
	XColor screen_color[OPACITY_MAX];
	XColor exact_color[OPACITY_MAX];
	#define MAX_COLOR_NAME_SIZE 256
	char *color_name[OPACITY_MAX];

	for (int i=0; i < OPACITY_MAX; i++) {
		color_name[i] = malloc(MAX_COLOR_NAME_SIZE+1);
		snprintf( color_name[i], MAX_COLOR_NAME_SIZE,
				"rgb:%02x/%02x/%02x",
				255*(i+1)/OPACITY_MAX,
				255*(i+1)/OPACITY_MAX,
				255*(i+1)/OPACITY_MAX
			);
		#ifdef DEBUG
		printf("Setup Color %i: %s\n", i, color_name[i]);
		#endif
		assert( XAllocNamedColor(dpy, colormap, color_name[i], &screen_color[i], &exact_color[i] ) );
		gc[i] = XCreateGC(dpy, w, 0, NULL);
		XSetForeground(dpy, gc[i], screen_color[i].pixel);
	}

	// Initialize the stars lib

	struct universe* u;
	new_universe( &u, w_attr.width, w_attr.height, Z_START );

	FRAME:
	XClearArea( dpy, w, 0, 0, w_attr.width, w_attr.height, 0 );
	for (;;) {
		struct return_point rp;
		int return_code = process_point( u, &rp  );
		if (return_code == 0) { break; }
		if (return_code == 1) {
			XDrawPoint( dpy, w, gc[rp.opacity], rp.x + (w_attr.width/2), rp.y + (w_attr.height/2));
		}
	}
	XFlush(dpy);
	if( rand()%100 < SPAWN_PERCENT ){
		for ( int c=0; c < (rand()%POSSIBLE_SPAWNS)+1; c++ ){ new_point( u ); }
	}
	usleep(U_SLEEP);
	goto FRAME;
	return 0;
}
