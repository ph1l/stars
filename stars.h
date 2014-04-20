#define WIDTH 1024
#define HEIGHT 768

#define MAX_Z 512
#define FRAME_DELAY 2

struct points
{
	int x, y, z;
	Uint8 r, g, b;
	struct points *next;
};

void new_point( struct points** );
void kill_point( struct points**, struct points* );
