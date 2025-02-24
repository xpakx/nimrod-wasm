#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include <common.h>
#include <canvas.h>

__attribute__((import_module("io_wasm"), import_name("jsprintf"))) 
void js_jsprintf(char* str);

void jsprintf(const char* format, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); 
	va_end(args);    
	js_jsprintf(buffer);
}

int width = 640;
int height = 480;

uint8_t* img = NULL;
size_t img_width = 0;
size_t img_height = 0;
int building_x = 2;
int building_y = 2;

uint8_t* pixel_data;

int mouseX = 0;
int mouseY = 0;
int isoX = 0;
int isoY = 0;

uint32_t map[ROWS][COLS];


int screenToIsoX(int x, int y) {
	float isoX = (float)x / DEFAULT_TILE_WIDTH + (float)y / DEFAULT_TILE_HEIGHT;
	return (int)ceil(isoX);
}

int screenToIsoY(int x, int y) {
	float isoY = (float)y / DEFAULT_TILE_HEIGHT - (float)x / DEFAULT_TILE_WIDTH;
	return (int)ceil(isoY);
}

int isoToScreenX(int x, int y) {
	return (x - y) * (DEFAULT_TILE_WIDTH / 2);
}

int isoToScreenY(int x, int y) {
	return (x + y) * (DEFAULT_TILE_HEIGHT / 2);
}

void setMouseIsoPosition(int x, int y) {
	float xTransl = x - (float)width / 2;
	float yTransl = y - (float)(height / 2 - (float)(DEFAULT_TILE_HEIGHT / 2));
	isoX = screenToIsoX(xTransl, yTransl);
	isoY = screenToIsoY(xTransl, yTransl);
	jsprintf("screen: (%d, %d)", x, y);
	jsprintf("iso: (%d, %d)", isoX, isoY);
}

void drawIsometricMap(uint8_t* buffer) {
	int translateX  = width / 2;
	int translateY  = height / 2 - (DEFAULT_TILE_HEIGHT/2);
	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLS; y++) {
			int screenX = translateX + isoToScreenX(x, y);
			int screenY = translateY + isoToScreenY(x, y);
			uint32_t color = map[x][y];
			if (isoX == x && isoY == y) {
				color = 0X000000FF;
			}
			drawTile(buffer, width, screenX, screenY, color);
		}
	}
}

void renderBuildings(uint8_t* buffer) {
	int translateX  = width / 2;
	int translateY  = height / 2 - (DEFAULT_TILE_HEIGHT/2);

	int screenX = translateX + isoToScreenX(building_x, building_y);
	int screenY = translateY + isoToScreenY(building_x, building_y);

	drawImage(img, img_width, img_height, pixel_data, width, height, screenX - img_width/2, screenY - img_height);
}

void drawMap(uint8_t* buffer) {
	drawIsometricMap(pixel_data);
	// TODO: render roads
	renderBuildings(pixel_data); // TODO: add pedestrians
}

void tick() {
	clearScreen(pixel_data, width, height);
	drawMap(pixel_data);
	js_draw_canvas((uint32_t)(uintptr_t)pixel_data, width * height * 4);
}

int init(int init_width, int init_height) {
	width = init_width;
	height = init_height;

	jsprintf("Allocating canvas object.");
	pixel_data = (uint8_t*)malloc(width * height * 4);
	if(pixel_data == NULL) {
		jsprintf("Allocating failed.");
		return 1;
	}

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			map[i][j] = 0x97B106FF;
		}
	}

	return 0;
}

int updateSize(int init_width, int init_height) {
	if (pixel_data != NULL) {
		jsprintf("Invaldating canvas object.");
		free(pixel_data);
		pixel_data = NULL;
	}
	return init(init_width, init_height);
}

void click(int x, int y) {
	if(x < 0 || x >= width) {
		return;
	}
	if(y < 0 || y >= width) {
		return;
	}
}

void sendImage(uint8_t* imageData, size_t inputWidth, size_t inputHeight) {
	size_t totalPixels = inputWidth * inputHeight;
	img = imageData;
	img_width = inputWidth;
	img_height = inputHeight;
}

void onMouseMove(int x, int y) {
	mouseX = x;
	mouseY = y;
	setMouseIsoPosition(x, y);
}

void onMouseClick(int button, int x, int y) {
	if (button == MOUSE_LEFT) {
		click(x, y);
	} else if (button == MOUSE_MIDDLE) {
		// TODO
	}
}

void onMouseUp(int button, int x, int y) {
	// TODO
}

void onMouseWheel(int deltaY) {
	// TODO
}

void onKeyDown(uint8_t keyCode) {
	switch (keyCode) {
		case LEFT:
			break;
		case RIGHT:
			break;
		case UP:
			break;
		case DOWN:
			break;
	}
}

void onKeyUp(uint8_t keyCode) {
	// TODO
}
