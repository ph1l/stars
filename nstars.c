#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "starslib.h"
#define MAX_Z 384

#define INPUT_PROMPT ">"
#define PROMPT_LEN 1
#define MAX_CMD_BUF_LEN 16

int main(int argc, char *argv[])
{
	WINDOW *win_stars, *win_status, *win_input;
	int c;
	int quit=0;

	char cmd_buf[MAX_CMD_BUF_LEN+1] = "";
	char *cmd_buf_p = (char *) &cmd_buf;
	int buf_len=0;
	int cur_loc=0;
	int x;

	struct universe* u;
	int frames=0;
	int sleep_time=3840;

	initscr();		/* start curses mode */
	raw();			/* disable line buffering */
	keypad(stdscr, TRUE);	/* enable arrows an function keys */
	noecho();		/* don't echo any input */
	curs_set(0);		/* set invisible cursor */
	nodelay(stdscr, TRUE);

	new_universe(&u, COLS, LINES-2, MAX_Z);

	refresh();
	win_stars = newwin(LINES-2, COLS, 0, 0);
	win_status = newwin(1, COLS, LINES-2, 0);
	win_input = newwin(1, COLS, LINES-1, 0);

	while(quit == 0){
		// draw stars
		wclear(win_stars);
		for (;;){
			struct return_point rp;
			int return_code = process_point(u, &rp);
			if (return_code == 0) break;
			if (return_code == 1)
				mvwprintw(win_stars,rp.y+((LINES-2)/2),rp.x+(COLS/2),"*");
		}
		frames++;
		wrefresh(win_stars);
		for (c=0; c< rand()%2;c++)
			new_point(u);
		usleep(sleep_time);

		// update status line
		mvwprintw(win_status,0,0,"[Console] f:%06d s:%06d", frames, sleep_time);
		wrefresh(win_status);

		// redraw input line
		buf_len = strlen(cmd_buf_p);
		mvwprintw(win_input, 0, 0, INPUT_PROMPT);
		mvwprintw(win_input, 0, PROMPT_LEN, "%s", &cmd_buf);
		for (x=PROMPT_LEN+buf_len; x<=COLS; x++)
			mvwaddch(win_input, 0, x, ' ');
		mvwchgat(win_input, 0, PROMPT_LEN+cur_loc, 1, A_REVERSE, 1, NULL);
		wrefresh(win_input);

		c = getch();
		if ( c == -1 ) continue;
		switch(c){
			/* ALT ESC? Doesn't work...
			case 27:
				c = getch();
				if (c == -1)
					quit = 1;
				break;
			*/
			case 258:					/* Down */
				beep();
				break;
			case 259:					/* Up */
				beep();
				break;
			case 260:					/* Left */
				if (cur_loc > 0)
					cur_loc--;
				else
					beep();
				break;
			case 261:					/* Right */
				if (cur_loc < buf_len)
					cur_loc++;
				else
					beep();
				break;
			case 8:						/* Ctrl-H */
			case 263:					/* Backspace */
				if (cur_loc > 0){
					for (x=cur_loc; x <= buf_len; x++)
						cmd_buf[x-1] = cmd_buf[x];
					cur_loc--;
				} else {
					beep();
				}
				break;
			case 10:					/* Enter */
				if (strcmp(cmd_buf_p,"/quit") == 0){
					quit = 1;
				} else if (strncmp(cmd_buf_p, "/sleep ", 7) == 0) {
					sscanf(cmd_buf_p, "/sleep %d", &sleep_time);
				}
				cmd_buf[0] = '\0';
				cur_loc=0;
				break;
			default:					/* Everything Else! */
				if (buf_len == MAX_CMD_BUF_LEN){
					beep();
					break;
				}
				cmd_buf[buf_len] = c;
				cmd_buf[buf_len+1] = '\0';
				cur_loc++;
		}
	}

	delwin(win_status);
	endwin();		/* end curses mode */

}
