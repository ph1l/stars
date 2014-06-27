#include <stdlib.h>
#include <stdio.h>
#include "starslib.h"

int main() {
	struct universe* u;
	new_universe( &u, 1024, 768, 512 );
	FRAME:
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