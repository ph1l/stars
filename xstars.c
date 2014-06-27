#include <X11/Xlib.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "starslib.h"

#define WIDTH 1024
#define HEIGHT 768

int main()
{
	// Open the display

	Display *dpy = XOpenDisplay(NULL);
	assert(dpy);


	int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

	// Get the root window

	Window w = DefaultRootWindow(dpy);

	// Initialize the stars lib
	struct universe* u;

	new_universe( &u, WIDTH, HEIGHT, 512 );


	// Create a "Graphics Context"

	GC gc = XCreateGC(dpy, w, 0, NULL);

	// Tell the GC we draw using the white color

	XSetForeground(dpy, gc, whiteColor);

	FRAME:
	XClearArea( dpy, w, 0, 0, WIDTH, HEIGHT, 0 );
	for (;;) {
		struct return_point rp;
		int return_code = process_point( u, &rp  );
		if (return_code == 0) { break; }
		if (return_code == 1) {
			XDrawPoint( dpy, w, gc, rp.x + (WIDTH/2), rp.y + (HEIGHT/2));
		}
	}
	XFlush(dpy);
	for ( int c=0; c<1; c++ ){ new_point( u ); }
	usleep(15000);
	goto FRAME;
	return 0;
}
