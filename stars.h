#define WIDTH 720
#define HEIGHT 480

#define MAX_Z 512
#define FRAME_DELAY 5

struct points
{
	int x, y, z;
	Uint8 r, g, b;
	struct points *next;
};

struct points* get_new_point();
