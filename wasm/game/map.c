#include <map.h>

void drawIsometricMap(Canvas* canvas, Pos* isoMouse, uint32_t map[ROWS][COLS]) {
	Pos iso;
	Pos screen;
	for (iso.x = 0; iso.x < ROWS; iso.x++) {
		for (iso.y = 0; iso.y < COLS; iso.y++) {
			isoToScreen(canvas, &iso, &screen);
			uint32_t color = map[iso.x][iso.y];
			if (isoMouse->x == iso.x && isoMouse->y == iso.y) {
				color = 0X000000FF;
			}
			drawTile(canvas, screen.x, screen.y, color);
		}
	}
}

void renderBuildings(Canvas* canvas, Building* building) {
	Pos iso;
	Pos screen;
	iso.x = building->pos.x;
	iso.y = building->pos.y;
	isoToScreen(canvas, &iso, &screen);

	drawImage(
		building->img->img, 
		building->img->width, 
		building->img->height,
		canvas, screen.x - building->img->width/2, screen.y - building->img->height
	);
}

void drawMap(Canvas* canvas, Pos* isoMouse, Building* building, uint32_t map[ROWS][COLS]) {
	drawIsometricMap(canvas, isoMouse, map);
	// TODO: render roads
	renderBuildings(canvas, building); // TODO: add pedestrians
}
