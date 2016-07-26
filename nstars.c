#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "starslib.h"
#define MAX_Z 512

int main(int argc, char *argv[])
{
	int c;
	struct universe* u;

	initscr();		/* start curses mode */
	noecho();		/* don't echo any input */
	curs_set(0);		/* hide the cursor */

	new_universe(&u, COLS, LINES, MAX_Z);

	while(1){
		clear();
		for (;;){
			struct return_point rp;
			int return_code = process_point(u, &rp);
			if (return_code == 0) break;
			if (return_code == 1)
				mvprintw(rp.y+(LINES/2),rp.x+(COLS/2),"*");
		}
		refresh();
		for (c=0; c< rand()%2;c++)
			new_point(u);
		usleep(10000);
	}

	endwin();		/* end curses mode */

}
