#ifndef GRAPHICS_H
#define GRAPHICS_H

__attribute__((import_module("io_wasm"), import_name("drawCanvas"))) 
void js_draw_canvas(uint32_t ptr, uint32_t length);

typedef struct {
    uint8_t* buffer;
    int width;
    int height;
} Canvas;

void clearScreen(Canvas* canvas);
void drawTile(Canvas* canvas, int x, int y, uint32_t color);
void drawImage(uint8_t* image, int imgWidth, int imgHeight, Canvas* canvas, int startX, int startY);

Canvas createCanvas(int width, int height);
void destroyCanvas(Canvas* canvas);
#endif // GRAPHICS_H
