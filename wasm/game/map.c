#include <map.h>

void drawIsometricMap(Canvas* canvas, Pos* isoMouse, MapLayer* map) {
	Pos iso;
	Pos screen;
	for (iso.x = 0; iso.x < map->width; iso.x++) {
		for (iso.y = 0; iso.y < map->height; iso.y++) {
			isoToScreen(canvas, &iso, &screen);
			uint32_t color = map->map[iso.x][iso.y];
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

void drawMap(Canvas* canvas, Pos* isoMouse, Building* building, MapLayer* map) {
	drawIsometricMap(canvas, isoMouse, map);
	// TODO: render roads
	renderBuildings(canvas, building); // TODO: add pedestrians
}

MapLayer createMapLayer(int width, int height) {
	MapLayer map;
	map.width = width;
	map.height = height;

	uint32_t **map_table = (uint32_t **)malloc(map.width * sizeof(uint32_t *));
	if (map_table == NULL) {
		map.map = NULL;
		return map;
	}

	for (int i = 0; i < map.width; i++) {
		map_table[i] = (uint32_t *)malloc(map.height * sizeof(uint32_t));
		if (map_table[i] == NULL) {
			for (int j = 0; j < i; j++) {
				free(map_table[j]);
			}
			free(map_table);
			map.map = NULL;
			return map;
		}
	}

	map.map = map_table;

	return map;
}

void destroyMapLayer(MapLayer* map) {
	for (int i = 0; i < map->width; i++) {
		free(map->map[i]);
	}
	free(map->map);
}
