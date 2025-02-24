#include <math.h>
#include <common.h>
#include <coord.h>

void screenToIso(Canvas* canvas, Pos* from, Pos* to) {
	float x = from->x - (float)canvas->width / 2;
	float y = from->y - (float)(canvas->height / 2 - (float)(DEFAULT_TILE_HEIGHT / 2));

	float isoX = (float)x / DEFAULT_TILE_WIDTH + (float)y / DEFAULT_TILE_HEIGHT;
	float isoY = (float)y / DEFAULT_TILE_HEIGHT - (float)x / DEFAULT_TILE_WIDTH;
	to->x = (int)ceil(isoX);
	to->y = (int)ceil(isoY);
}

int isoToScreenX(int x, int y) {
	return (x - y) * (DEFAULT_TILE_WIDTH / 2);
}

int isoToScreenY(int x, int y) {
	return (x + y) * (DEFAULT_TILE_HEIGHT / 2);
}

void isoToScreen(Canvas* canvas, Pos* from, Pos* to) {
	int translateX  = canvas->width / 2;
	int translateY  = canvas->height / 2 - (DEFAULT_TILE_HEIGHT/2);
	int x = (from->x - from->y) * (DEFAULT_TILE_WIDTH / 2);
	int y = (from->x + from->y) * (DEFAULT_TILE_HEIGHT / 2);
	to->x = translateX + x;
	to->y = translateY + y;
}
