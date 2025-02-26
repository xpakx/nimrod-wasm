#ifndef MAP_H
#define MAP_H

#include <canvas.h>
#include <coord.h>
#include <common.h>

void drawMap(Canvas* canvas, Pos* isoMouse, Pos* building, size_t img_width, size_t img_height, uint8_t* img, uint32_t map[ROWS][COLS]);

#endif // MAP_H
