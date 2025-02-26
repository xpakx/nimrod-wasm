#ifndef MAP_H
#define MAP_H

#include <canvas.h>
#include <coord.h>
#include <common.h>
#include <building.h>

void drawMap(Canvas* canvas, Pos* isoMouse, Building* building, uint32_t map[ROWS][COLS]);

#endif // MAP_H
