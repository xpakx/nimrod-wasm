#include <building.h>

ImageSprite createSprite(uint8_t* img, int width, int height) {
    ImageSprite sprite;
    sprite.width = width;
    sprite.height = height;
    sprite.img = img;
    return sprite;
}

void destroySprite(ImageSprite* sprite) {
    if (sprite->img) {
        free(sprite->img);
        sprite->img = NULL;
    }
}
