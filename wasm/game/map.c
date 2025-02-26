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

void renderBuildings(Canvas* canvas, Pos* building, size_t img_width, size_t img_height, uint8_t* img) {
	Pos iso;
	Pos screen;
	iso.x = building->x;
	iso.y = building->y;
	isoToScreen(canvas, &iso, &screen);

	drawImage(img, img_width, img_height, canvas, screen.x - img_width/2, screen.y - img_height);
}

void drawMap(Canvas* canvas, Pos* isoMouse, Pos* building, size_t img_width, size_t img_height, uint8_t* img, uint32_t map[ROWS][COLS]) {
	drawIsometricMap(canvas, isoMouse, map);
	// TODO: render roads
	renderBuildings(canvas, building, img_width, img_height, img); // TODO: add pedestrians
}
