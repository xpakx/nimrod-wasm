#ifndef MAP_H
#define MAP_H

#include <canvas.h>
#include <coord.h>
#include <common.h>
#include <building.h>
#include <array.h>

typedef struct {
    uint32_t **map;
    int width;
    int height;
} MapLayer;

void drawMap(Canvas* canvas, Pos* isoMouse, Array* buildings, MapLayer* map);
MapLayer createMapLayer(int width, int height);
void destroyMapLayer(MapLayer* map);

#endif // MAP_H
