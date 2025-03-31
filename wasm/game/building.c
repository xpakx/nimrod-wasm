#include <building.h>

ImageSprite* createSprite(uint8_t* img, int width, int height) {
	ImageSprite* sprite = malloc(sizeof(ImageSprite));
	sprite->width = width;
	sprite->height = height;
	sprite->img = img;
	return sprite;
}

void destroySprite(ImageSprite* sprite) {
    if (sprite->img) {
        free(sprite->img);
        sprite->img = NULL;
    }
    free(sprite);
}

int scaleToIndex(int scale) {
	return scale - MIN_SCALE;
}
// TODO
ImageSprite* getImgForScale(Building* building, int scale) {
	int index = scaleToIndex(scale);
	return building->sprites[index];
}
