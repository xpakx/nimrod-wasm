#ifndef COORD_H
#define COORD_H

#include <canvas.h>

typedef struct {
    int x;
    int y;
} Pos;

void screenToIso(Canvas* canvas, Pos* from, Pos* to);
void isoToScreen(Canvas* canvas, Pos* from, Pos* to);

#endif // COORD_H
