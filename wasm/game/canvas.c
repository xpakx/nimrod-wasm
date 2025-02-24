#include <stdlib.h>
#include <math.h>

#include <common.h>
#include <canvas.h>

void fillPixel(uint8_t* buffer, int index, int color) {
	buffer[index] = (color >> 24) & 0xFF;
	buffer[index + 1] = (color >> 16) & 0xFF;
	buffer[index + 2] = (color >> 8) & 0xFF;
	buffer[index + 3] = color & 0xFF;
}

void clearScreen(Canvas* canvas) {
	int height = canvas->height;
	int width = canvas->width;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x) * 4;
			fillPixel(canvas->buffer, index, 0X1E1E2EFF);
		}
	}
}

uint8_t alpha_blend(uint8_t src, uint8_t dest, float alpha) {
	return (uint8_t)(src * alpha + dest * (1 - alpha));
}

void drawRow(Canvas* canvas, int y, int startX, int endX, uint32_t color) {
	int width = canvas->width;
        for (int dx = startX + 1; dx <= endX - 1; dx++) { // +1 and -1 just for debugging
            int index = (y * width + dx) * 4;
            fillPixel(canvas->buffer, index, color);
        }
}

void drawTile(Canvas* canvas, int x, int y, uint32_t color) {
	int halfWidth = DEFAULT_TILE_WIDTH / 2;
	int halfHeight = DEFAULT_TILE_HEIGHT / 2;

	int startX = x;
	int endX = x;
	int step = DEFAULT_TILE_WIDTH / DEFAULT_TILE_HEIGHT;
	for (int dy = 0; dy < halfHeight; dy++) {
		startX -= step;
		endX += step;
		drawRow(canvas, y - dy, startX, endX, color);
	}
	for (int dy = halfHeight; dy < DEFAULT_TILE_HEIGHT; dy++) {
		startX += step;
		endX -= step;
		drawRow(canvas, y - dy, startX, endX, color);
	}
}

void drawImage(uint8_t* image, int imgWidth, int imgHeight, Canvas* canvas, int startX, int startY) {
	if (image == NULL) {
		return;
	}
	int width = canvas->width;
	int height = canvas->height;
	uint8_t* buffer = canvas->buffer;
	for (int y = 0; y < imgHeight; y++) {
		for (int x = 0; x < imgWidth; x++) {
			int index = (y * imgWidth + x) * 4;
			int buffer_index = ((y + startY) * width + x + startX) * 4;

			float alpha = image[index + 3] / 255.0f;

			buffer[buffer_index] = alpha_blend(image[index], buffer[buffer_index], alpha);
			buffer[buffer_index + 1] = alpha_blend(image[index + 1], buffer[buffer_index + 1], alpha);
			buffer[buffer_index + 2] = alpha_blend(image[index + 2], buffer[buffer_index + 2], alpha);
			buffer[buffer_index + 3] = (uint8_t)(image[index + 3] + buffer[buffer_index + 3] * (1 - alpha));
		}
	}
}

Canvas createCanvas(int width, int height) {
    Canvas canvas;
    canvas.width = width;
    canvas.height = height;
    canvas.buffer = (uint8_t*)malloc(width * height * 4);
    return canvas;
}

void destroyCanvas(Canvas* canvas) {
    if (canvas->buffer) {
        free(canvas->buffer);
        canvas->buffer = NULL;
    }
}
