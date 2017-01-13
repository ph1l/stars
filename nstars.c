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

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "starslib.h"
#define MAX_Z 384

#define INPUT_PROMPT ">"
#define PROMPT_LEN 1
#define MAX_CMD_BUF_LEN 16
#define MAX_CMD_HIST_LEN 4

#define DEFAULT_STAR '.'

#define HELP "\
Usage: %s [OPTION]...\n\
Starfield with ncurses frontend\n\
\n\
  -h                       display this help and exit\n\
"

enum COLORMODE {
  CM_MONO,
  CM_256
} colormode = CM_MONO;
#define STARCOLOR(opacity) (232+(256-232-1)*opacity/OPACITY_MAX)

void init256color()
{
	static bool inited = false;

	if (inited)
		return;

	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "terminal does not support colors");
		exit(EXIT_FAILURE);
	}

	start_color();
	for (int i=0; i<=OPACITY_MAX; i++)
		init_pair(STARCOLOR(i), STARCOLOR(i), COLOR_BLACK);

	inited = true;
}

int main(int argc, char *argv[])
{
	WINDOW *win_stars, *win_status, *win_input;
	int c, x;
	int quit = 0;

	char cmd_buf[MAX_CMD_BUF_LEN+1] = "";
	char *cmd_buf_p = (char *) &cmd_buf;
	int buf_len = 0;
	int cur_loc = 0;

	char *cmd_hist[MAX_CMD_HIST_LEN];
	cmd_hist[0] = NULL;
	int cmd_hist_loc = -1;

	struct universe* u;
	int frames = 0;
	int sleep_time = 3840;
	char star_char = DEFAULT_STAR;

	// parse command-line options
	int opt;
	while ((opt=getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				fprintf(stderr, HELP, argv[0]);
				exit(EXIT_SUCCESS);
			default:
				fprintf(stderr, HELP, argv[0]);
				exit(EXIT_FAILURE);
		}
	}

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

	while (quit == 0) {
		// draw stars
		wclear(win_stars);
		for (;;) {
			struct return_point rp;
			int return_code = process_point(u, &rp);
			if (return_code == 0) break;
			if (return_code == 1) {
				if (colormode == CM_256)
					wattron(win_stars, COLOR_PAIR(STARCOLOR(rp.opacity)));
				mvwprintw(win_stars,rp.y+((LINES-2)/2),rp.x+(COLS/2),"%c",star_char);
				if (colormode == CM_256)
					wattroff(win_stars, COLOR_PAIR(STARCOLOR(rp.opacity)));
			}
		}
		frames++;
		wrefresh(win_stars);
		for (c=0; c < rand()%2; c++)
			new_point(u);
		usleep(sleep_time);

		// update status line
		mvwprintw(win_status, 0, 0, "[Console] f:%06d s:%06d", frames, sleep_time);
		wrefresh(win_status);

		// redraw input line
		buf_len = strlen(cmd_buf_p);
		mvwprintw(win_input, 0, 0, INPUT_PROMPT);
		mvwprintw(win_input, 0, PROMPT_LEN, "%s", &cmd_buf);
		for (x=PROMPT_LEN+buf_len; x <= COLS; x++)
			mvwaddch(win_input, 0, x, ' ');
		mvwchgat(win_input, 0, PROMPT_LEN+cur_loc, 1, A_REVERSE, 1, NULL);
		wrefresh(win_input);

		c = getch();

		if (c == -1) continue;

		switch (c) {
			/* ALT ESC? Doesn't work...
			case 27:
				c = getch();
				if (c == -1)
					quit = 1;
				break;
			*/
			case KEY_DOWN:
				if (cmd_hist_loc > -1){
					cmd_hist_loc--;
					if (cmd_hist_loc == -1) {
						cmd_buf[0] = '\0';
						cur_loc=0;
					} else {
						strncpy(cmd_buf_p, cmd_hist[cmd_hist_loc], MAX_CMD_BUF_LEN);
						cur_loc=strlen(cmd_buf_p);
					}
				} else {
					beep();
				}
				break;
			case KEY_UP:
				if (
					(cmd_hist_loc == -1 && cmd_hist[0] != NULL)
					||
					(
						cmd_hist_loc >=0 &&
						cmd_hist_loc < (MAX_CMD_HIST_LEN-1) &&
						cmd_hist[cmd_hist_loc+1] != NULL
					)
				) {
					cmd_hist_loc++;
					strncpy(cmd_buf_p, cmd_hist[cmd_hist_loc], MAX_CMD_BUF_LEN);
					cur_loc=strlen(cmd_buf_p);
				} else {
					beep();
				}
				break;
			case KEY_LEFT:
				if (cur_loc > 0)
					cur_loc--;
				else
					beep();
				break;
			case KEY_RIGHT:
				if (cur_loc < buf_len)
					cur_loc++;
				else
					beep();
				break;
			case 8:						/* Ctrl-H */
			case KEY_BACKSPACE:
				if (cur_loc > 0) {
					for (x=cur_loc; x <= buf_len; x++)
						cmd_buf[x-1] = cmd_buf[x];
					cur_loc--;
				} else {
					beep();
				}
				break;
			case '\n':					/* Line Feed (0x0A) */
			case '\r':					/* Carriage Return (0x0D) */
			case KEY_ENTER:
				if (strncmp(cmd_buf_p, "/quit", MAX_CMD_BUF_LEN) == 0) {
					quit = 1;
				} else if (strncmp(cmd_buf_p, "/sleep ", 7) == 0) {
					sscanf(cmd_buf_p, "/sleep %d", &sleep_time);
				} else if (	strlen(cmd_buf_p) == 7 &&
						strncmp(cmd_buf_p, "/char ", 6) == 0
						) {
					sscanf(cmd_buf_p, "/char %c", &star_char);
				} else if (strncmp(cmd_buf_p, "/colormode ", 11) == 0) {
					if (strcmp(cmd_buf_p+11, "mono") == 0) {
						colormode = CM_MONO;
					} else if (strcmp(cmd_buf_p+11, "256") == 0) {
						init256color();
						colormode = CM_256;
					}
				}
				/* save command to history */
				if (	cmd_hist[0] == NULL ||
					strncmp(cmd_buf_p, cmd_hist[0], MAX_CMD_BUF_LEN) != 0
					) {

					for (x=0; cmd_hist[x] != NULL && x < (MAX_CMD_HIST_LEN-1); x++);
					if (x == (MAX_CMD_HIST_LEN-1))
						free(cmd_hist[x]);
					else
						cmd_hist[x+1] = NULL;
					for (;x > 0; x--)
						cmd_hist[x] = cmd_hist[x-1];

					cmd_hist[0] = strndup(cmd_buf_p, MAX_CMD_BUF_LEN);
				}

				/* reset command buffer */
				cmd_buf[0] = '\0';
				cur_loc = 0;
				cmd_hist_loc = -1;
				break;
			default:					/* Everything Else! */
				if (buf_len == MAX_CMD_BUF_LEN) {
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
