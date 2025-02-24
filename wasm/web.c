#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include <common.h>
#include <canvas.h>
#include <coord.h>

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
int building_x = 2;
int building_y = 2;

Pos mouse;
Pos isoMouse;

uint32_t map[ROWS][COLS];

void drawIsometricMap(Canvas* canvas) {
	Pos iso;
	Pos screen;
	for (iso.x = 0; iso.x < ROWS; iso.x++) {
		for (iso.y = 0; iso.y < COLS; iso.y++) {
			isoToScreen(canvas, &iso, &screen);
			uint32_t color = map[iso.x][iso.y];
			if (isoMouse.x == iso.x && isoMouse.y == iso.y) {
				color = 0X000000FF;
			}
			drawTile(canvas, screen.x, screen.y, color);
		}
	}
}

void renderBuildings(Canvas* canvas) {
	Pos iso;
	Pos screen;
	iso.x = building_x;
	iso.y = building_y;
	isoToScreen(canvas, &iso, &screen);

	drawImage(img, img_width, img_height, canvas, screen.x - img_width/2, screen.y - img_height);
}

void drawMap(Canvas* canvas) {
	drawIsometricMap(canvas);
	// TODO: render roads
	renderBuildings(canvas); // TODO: add pedestrians
}

void tick() {
	clearScreen(&canvas);
	drawMap(&canvas);
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
