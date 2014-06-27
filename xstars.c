#include <X11/Xlib.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "starslib.h"

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

	int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
	GC gc = XCreateGC(dpy, w, 0, NULL);
	XSetForeground(dpy, gc, whiteColor);

	// Initialize the stars lib

	struct universe* u;
	new_universe( &u, w_attr.width, w_attr.height, 512 );

	FRAME:
	XClearArea( dpy, w, 0, 0, w_attr.width, w_attr.height, 0 );
	for (;;) {
		struct return_point rp;
		int return_code = process_point( u, &rp  );
		if (return_code == 0) { break; }
		if (return_code == 1) {
			XDrawPoint( dpy, w, gc, rp.x + (w_attr.width/2), rp.y + (w_attr.height/2));
		}
	}
	XFlush(dpy);
	for ( int c=0; c<1; c++ ){ new_point( u ); }
	usleep(15000);
	goto FRAME;
	return 0;
}
