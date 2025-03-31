#include <math.h>
#include <common.h>
#include <coord.h>

void screenToIso(Canvas* canvas, Pos* from, Pos* to) {
	float x = from->x - (float)canvas->width / 2;
	float y = from->y - (float)(canvas->height / 2 - (float)(canvas->tileHeight / 2));

	float isoX = (float)x / canvas->tileWidth + (float)y / canvas->tileHeight;
	float isoY = (float)y / canvas->tileHeight - (float)x / canvas->tileWidth;
	to->x = (int)ceil(isoX);
	to->y = (int)ceil(isoY);
}

int isoToScreenX(int x, int y, int tileWidth) {
	return (x - y) * (tileWidth / 2);
}

int isoToScreenY(int x, int y, int tileHeight) {
	return (x + y) * (tileHeight / 2);
}

void isoToScreen(Canvas* canvas, Pos* from, Pos* to) {
	int translateX  = canvas->width / 2;
	int translateY  = canvas->height / 2 - (canvas->tileHeight/2);
	int x = (from->x - from->y) * (canvas->tileWidth / 2);
	int y = (from->x + from->y) * (canvas->tileHeight / 2);
	to->x = translateX + x;
	to->y = translateY + y;
}
