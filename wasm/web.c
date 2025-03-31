#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include <common.h>
#include <canvas.h>
#include <coord.h>
#include <map.h>
#include <building.h>
#include <array.h>

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

Canvas canvas;
ImageSprite img;
Array sprites;

Pos mouse;
Pos isoMouse;

MapLayer mapLayer;
int scale = 5;

void tick() {
	clearScreen(&canvas);
	drawMap(&canvas, &isoMouse, &mapLayer);
	js_draw_canvas((uint32_t)(uintptr_t)canvas.buffer, canvas.width * canvas.height * 4);
}

int declareCanvas(int init_width, int init_height) {
	jsprintf("Allocating canvas object.");
	canvas  = createCanvas(init_width, init_height);
	if(canvas.buffer == NULL) {
		jsprintf("Allocating failed.");
		return 1;
	}

	return 0;
}

int declareMap(int width, int height) {
	mapLayer = createMapLayer(width, height);
	if (mapLayer.map == NULL) {
		jsprintf("Allocating memory for map layer failed.");
		return 1;
	}
	for (int i = 0; i < mapLayer.width; i++) {
		for (int j = 0; j < mapLayer.height; j++) {
			mapLayer.map[i][j] = 0xD8BE5DFF;
		}
	}
	createArray(&mapLayer.buildings, 2);
	createArray(&sprites, 2);
	return 0;
}

int init(int init_width, int init_height) {
	mouse.x = 0;
	mouse.y = 0;
	isoMouse.x = 0;
	isoMouse.y = 0;

	int can = declareCanvas(init_width, init_height);
	if (can == 1) {
		return 1;
	}
	return declareMap(ROWS, COLS);
}

int updateSize(int init_width, int init_height) {
	jsprintf("Invaldating canvas object.");
	destroyCanvas(&canvas);
	return declareCanvas(init_width, init_height);
}

void click(int x, int y) {
	if(x < 0 || x >= canvas.width) {
		return;
	}
	if(y < 0 || y >= canvas.width) {
		return;
	}
}

int sendImage(uint8_t* imageData, size_t inputWidth, size_t inputHeight) {
	ImageSprite*  sprite = createSprite(imageData, inputWidth, inputHeight);
	push(&sprites, sprite);
	return sprites.size - 1;
}

// TODO: building prototype
void createBuilding(int x, int y, int img3, int img4, int img5, int img6, int img7, int img8, int img9, int img10) {
	Building* building = malloc(sizeof(Building));
	building->sprites[0] = getElement(&sprites, img3);
	building->sprites[1] = getElement(&sprites, img4);
	building->sprites[2] = getElement(&sprites, img5);
	building->sprites[3] = getElement(&sprites, img6);
	building->sprites[4] = getElement(&sprites, img7);
	building->sprites[5] = getElement(&sprites, img8);
	building->sprites[6] = getElement(&sprites, img9);
	building->sprites[7] = getElement(&sprites, img10);
	building->pos.x = x;
	building->pos.y = y;
	push(&mapLayer.buildings, building);

	for (size_t i = 0; i < mapLayer.buildings.size; i++) {
		Building* b = (Building*)getElement(&mapLayer.buildings, i);
		jsprintf("Building %zu: Position (%d, %d)\n", i, b->pos.x, b->pos.y);
	}
}

void onMouseMove(int x, int y) {
	mouse.x = x;
	mouse.y = y;
	screenToIso(&canvas, &mouse, &isoMouse);
	jsprintf("screen: (%d, %d)", mouse.x, mouse.y);
	jsprintf("iso: (%d, %d)", isoMouse.x, isoMouse.y);
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

void changeScale(int delta) {
	scale += delta;
	if (scale < MIN_SCALE) {
		scale = MIN_SCALE;
	}
	if (scale > MAX_SCALE) {
		scale = MAX_SCALE;
	}
	rescaleMap(&mapLayer, scale);
	rescaleTiles(&canvas, scale);
}

void onMouseWheel(int deltaY) {
	if (deltaY > 0) {
		changeScale(-1);
	} else {
		changeScale(1);
	}
	jsprintf("scale: %d", scale);
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
