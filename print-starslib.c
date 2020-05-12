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

#include <stdlib.h>
#include <stdio.h>
#include "starslib.h"

int main() {
	int f_count=0;
	struct universe* u;
	new_universe( &u, 1024, 768, 512 );
	FRAME:
	f_count++;
	printf("FRAME: %i\n", f_count);
	for (;;) {
		struct return_point rp;
		int return_code = process_point( u, &rp  );
		if (return_code == 0) { break; }
		if (return_code == 1) { printf("draw pix @ %i, %i\n", rp.x, rp.y); }
	}
	for ( int c=0; c<1; c++ ){ new_point( u ); }
	goto FRAME;
	return 0;
}
