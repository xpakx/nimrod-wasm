__attribute__((import_module("io_wasm"), import_name("drawCanvas"))) 
void js_draw_canvas(uint32_t ptr, uint32_t length);

void clearScreen(uint8_t* buffer, int width, int height);
void drawTile(uint8_t* buffer, int width, int x, int y, uint32_t color);
void drawImage(uint8_t* image, int imgWidth, int imgHeight, uint8_t* buffer, int width, int height, int startX, int startY);

