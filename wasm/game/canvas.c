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

uint8_t alphaBlend(uint8_t src, uint8_t dest, float alpha) {
	return (uint8_t)(src * alpha + dest * (1 - alpha));
}

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

void drawRow(Canvas* canvas, int y, int startX, int endX, uint32_t color) {
	int width = canvas->width; 
        for (int dx = startX; dx <= endX; dx++) {
            int index = (y * width + dx) * 4;
            fillPixel(canvas->buffer, index, color);
        }
}

void drawTile(Canvas* canvas, int x, int y, uint32_t color, int width, int height) {
	int halfWidth = width / 2;
	int halfHeight = height / 2;

	int startX = x;
	int endX = x;
	int step = width / height;
	int cwidth = canvas->width-1;
	for (int dy = 0; dy < halfHeight; dy++) {
		startX -= step;
		endX += step;
		drawRow(canvas, y - dy, max(0, startX), min(cwidth, endX), color);
	}
	for (int dy = halfHeight; dy < height; dy++) {
		startX += step;
		endX -= step;
		drawRow(canvas, y - dy, max(0, startX), min(cwidth, endX), color);
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

			buffer[buffer_index] = alphaBlend(image[index], buffer[buffer_index], alpha);
			buffer[buffer_index + 1] = alphaBlend(image[index + 1], buffer[buffer_index + 1], alpha);
			buffer[buffer_index + 2] = alphaBlend(image[index + 2], buffer[buffer_index + 2], alpha);
			buffer[buffer_index + 3] = (uint8_t)(image[index + 3] + buffer[buffer_index + 3] * (1 - alpha));
		}
	}
}

Canvas createCanvas(int width, int height) {
    Canvas canvas;
    canvas.width = width;
    canvas.height = height;
    canvas.buffer = (uint8_t*)malloc(width * height * 4);
    canvas.tileWidth = DEFAULT_TILE_WIDTH;
    canvas.tileHeight = DEFAULT_TILE_HEIGHT;
    return canvas;
}

void destroyCanvas(Canvas* canvas) {
    if (canvas->buffer) {
        free(canvas->buffer);
        canvas->buffer = NULL;
    }
}

void rescaleTiles(Canvas* canvas, int newScale) {
	float scale = newScale * 0.2f;
	canvas->tileWidth = (float)DEFAULT_TILE_WIDTH * scale;
	canvas->tileHeight = (float)DEFAULT_TILE_HEIGHT * scale;
}
