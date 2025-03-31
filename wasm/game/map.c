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
			drawTile(canvas, screen.x, screen.y, color, map->tileWidth, map->tileHeight);
		}
	}
}

void renderBuildings(Canvas* canvas, Array* buildings, int scale) {
	Pos iso;
	Pos screen;
	for (size_t i = 0; i < buildings->size; i++) {
		Building* building = (Building*)getElement(buildings, i);
		iso.x = building->pos.x;
		iso.y = building->pos.y;
		isoToScreen(canvas, &iso, &screen);
		ImageSprite* img = getImgForScale(building, scale);
		drawImage(
				img->img, 
				img->width, 
				img->height,
				canvas, screen.x - img->width/2, screen.y - img->height
			 );
	}

}

void rescaleMap(MapLayer* map, int newScale) {
	float scale = newScale * 0.2f;
	map->tileWidth = (float)DEFAULT_TILE_WIDTH * scale;
	map->tileHeight = (float)DEFAULT_TILE_HEIGHT * scale;
	map->scale = newScale;
}

void drawMap(Canvas* canvas, Pos* isoMouse, MapLayer* map) {
	drawIsometricMap(canvas, isoMouse, map);
	// TODO: render roads
	renderBuildings(canvas, &map->buildings, map->scale); // TODO: add pedestrians
}

MapLayer createMapLayer(int width, int height) {
	MapLayer map;
	map.width = width;
	map.height = height;

	map.map = (uint32_t **)malloc(map.width * sizeof(uint32_t *));
	if (map.map == NULL) {
		return map;
	}

	map.map[0] = (uint32_t *)malloc(map.width * map.height * sizeof(uint32_t));
	if (map.map[0] == NULL) {
		free(map.map);
		map.map = NULL;
		return map;
	}

	for (int i = 1; i < map.width; i++) {
		map.map[i] = map.map[0] + i * map.height;
	}
	map.tileWidth = DEFAULT_TILE_WIDTH;
	map.tileHeight = DEFAULT_TILE_HEIGHT;
	map.scale = 5;

	return map;
}

void destroyMapLayer(MapLayer* map) {
	if (map->map[0] != NULL) {
		free(map->map[0]);
	}
	free(map->map);
}
