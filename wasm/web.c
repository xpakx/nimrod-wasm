#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include <common.h>
#include <canvas.h>
#include <coord.h>
#include <map.h>

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

uint8_t* img = NULL;
size_t img_width = 0;
size_t img_height = 0;

Pos mouse;
Pos isoMouse;
Pos building;

uint32_t map[ROWS][COLS];


void tick() {
	clearScreen(&canvas);
	drawMap(&canvas, &isoMouse, &building, img_width, img_height, img, map);
	js_draw_canvas((uint32_t)(uintptr_t)canvas.buffer, canvas.width * canvas.height * 4);
}

int declareCanvas(int init_width, int init_height) {
	jsprintf("Allocating canvas object.");
	canvas  = createCanvas(init_width, init_height);
	if(canvas.buffer == NULL) {
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

int init(int init_width, int init_height) {
	mouse.x = 0;
	mouse.y = 0;
	isoMouse.x = 0;
	isoMouse.y = 0;
	building.x = 2;
	building.y = 2;
	return declareCanvas(init_width, init_height);
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

void sendImage(uint8_t* imageData, size_t inputWidth, size_t inputHeight) {
	size_t totalPixels = inputWidth * inputHeight;
	img = imageData;
	img_width = inputWidth;
	img_height = inputHeight;
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
