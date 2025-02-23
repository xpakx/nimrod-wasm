#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1

#define DEFAULT_TILE_WIDTH 64
#define DEFAULT_TILE_HEIGHT 32
#define ROWS 4
#define COLS 4

__attribute__((import_module("io_wasm"), import_name("jsprintf"))) 
void js_jsprintf(char* str);

__attribute__((import_module("io_wasm"), import_name("drawCanvas"))) 
void js_draw_canvas(uint32_t ptr, uint32_t length);

void jsprintf(const char* format, ...) {
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); 
	va_end(args);    
	js_jsprintf(buffer);
}

float ball_x = 100.0f;
float ball_y = 100.0f;
float ball_radius = 30.0f;
float ball_speed_x = 2.0f;
float ball_speed_y = 1.5f;
int ball_color = 0X9399B2FF;
int width = 640;
int height = 480;
uint8_t* img = NULL;
size_t img_width = 0;
size_t img_height = 0;

uint8_t* pixel_data;

int mouseX = 0;
int mouseY = 0;

uint32_t map[ROWS][COLS];

void fillPixel(uint8_t* buffer, int index, int color) {
	buffer[index] = (color >> 24) & 0xFF;
	buffer[index + 1] = (color >> 16) & 0xFF;
	buffer[index + 2] = (color >> 8) & 0xFF;
	buffer[index + 3] = color & 0xFF;
}

void clearScreen(uint8_t* buffer) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = (y * width + x) * 4;
			fillPixel(buffer, index, 0X1E1E2EFF);
		}
	}
}

void draw_ball(uint8_t* buffer) {
	int x0 = (int)ball_x;
	int y0 = (int)ball_y;
	int r = (int)ball_radius;

	for (int y = -r; y <= r; ++y) {
		for (int x = -r; x <= r; ++x) {
			if (x*x + y*y <= r*r) {
				int px = x0 + x;
				int py = y0 + y;

				if (px >= 0 && px < width && py >= 0 && py < height) {
					int index = (py * width + px) * 4;
					fillPixel(buffer, index, ball_color);
				}
			}
		}
	}
}

void update_ball_position() {
	ball_x += ball_speed_x;
	ball_y += ball_speed_y;

	if (ball_x < ball_radius || ball_x > width - ball_radius) {
		ball_speed_x = -ball_speed_x;
	}
	if (ball_y < ball_radius || ball_y > height - ball_radius) {
		ball_speed_y = -ball_speed_y;
	}
}

uint8_t alpha_blend(uint8_t src, uint8_t dest, float alpha) {
	return (uint8_t)(src * alpha + dest * (1 - alpha));
}

void draw_img(uint8_t* buffer) {
	if (img == NULL) {
		return;
	}
	for (int y = 0; y < img_height; y++) {
		for (int x = 0; x < img_width; x++) {
			int index = (y * img_width + x) * 4;
			int buffer_index = (y * width + x) * 4;

			float alpha = img[index + 3] / 255.0f;

			buffer[buffer_index] = alpha_blend(img[index], buffer[buffer_index], alpha);
			buffer[buffer_index + 1] = alpha_blend(img[index + 1], buffer[buffer_index + 1], alpha);
			buffer[buffer_index + 2] = alpha_blend(img[index + 2], buffer[buffer_index + 2], alpha);
			buffer[buffer_index + 3] = (uint8_t)(img[index + 3] + buffer[buffer_index + 3] * (1 - alpha));
		}
	}
}

void drawRow(uint8_t* buffer, int y, int startX, int endX, uint32_t color) {
        for (int dx = startX + 1; dx <= endX - 1; dx++) { // +1 and -1 just for debugging
            int index = (y * width + dx) * 4;
            fillPixel(buffer, index, color);
        }
}

void drawTile(uint8_t* buffer, int x, int y, uint32_t color) {
    int halfWidth = DEFAULT_TILE_WIDTH / 2;
    int halfHeight = DEFAULT_TILE_HEIGHT / 2;

    int startX = x;
    int endX = x;
    int step = DEFAULT_TILE_WIDTH / DEFAULT_TILE_HEIGHT;
    for (int dy = 0; dy < halfHeight; dy++) {
	startX -= step;
	endX += step;
	drawRow(buffer, y - dy, startX, endX, color);
    }
    for (int dy = halfHeight; dy < DEFAULT_TILE_HEIGHT; dy++) {
	startX += step;
	endX -= step;
	drawRow(buffer, y - dy, startX, endX, color);
    }
}

void drawIsometricMap() {
	int translateX  = width / 2;
	int translateY  = height / 2 - (DEFAULT_TILE_HEIGHT/2);
	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLS; y++) {
			int screenX = translateX + ((x - y) * (DEFAULT_TILE_WIDTH / 2));
			int screenY = translateY + ((x + y) * (DEFAULT_TILE_HEIGHT / 2));
			drawTile(pixel_data, screenX, screenY, map[x][y]);
		}
	}
}

void tick() {
	update_ball_position();
	clearScreen(pixel_data);
	drawIsometricMap();
	draw_ball(pixel_data);
	draw_img(pixel_data);
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


int in_ball(int click_x, int click_y) {
	int x0 = (int)ball_x;
	int y0 = (int)ball_y;

	float d = sqrtf(pow(click_x - x0, 2) + pow(click_y - y0, 2));
	jsprintf("distance: %.2f", d);
	if(d < ball_radius) {
		return 1;
	}
	return 0;
}

void click(int x, int y) {
	if(x < 0 || x >= width) {
		return;
	}
	if(y < 0 || y >= width) {
		return;
	}

	if(in_ball(x, y) == 1) {
		if(ball_color == 0x9399B2FF) {
			ball_color = 0xF2CDCDFF;
		} else {
			ball_color = 0x9399B2FF;
		}
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
			if(ball_speed_x > 0) {
				ball_speed_x = -ball_speed_x;
			}
			break;
		case RIGHT:
			if(ball_speed_x < 0) {
				ball_speed_x = -ball_speed_x;
			}
			break;
		case UP:
			if(ball_speed_y > 0) {
				ball_speed_y = -ball_speed_y;
			}
			break;
		case DOWN:
			if(ball_speed_y < 0) {
				ball_speed_y = -ball_speed_y;
			}
			break;
	}
}

void onKeyUp(uint8_t keyCode) {
	// TODO
}
