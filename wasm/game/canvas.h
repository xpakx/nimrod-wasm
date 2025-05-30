#ifndef CANVAS_H
#define CANVAS_H

#include <stdlib.h>

__attribute__((import_module("io_wasm"), import_name("drawCanvas"))) 
void js_draw_canvas(uint32_t ptr, uint32_t length);

typedef struct {
    uint8_t* buffer;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
} Canvas;

void clearScreen(Canvas* canvas);
void drawTile(Canvas* canvas, int x, int y, uint32_t color, int width, int height);
void drawImage(uint8_t* image, int imgWidth, int imgHeight, Canvas* canvas, int startX, int startY);

Canvas createCanvas(int width, int height);
void destroyCanvas(Canvas* canvas);
void rescaleTiles(Canvas* canvas, int newScale);

#endif // CANVAS_H
