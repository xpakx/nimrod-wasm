#ifndef BUILDING_H
#define BUILDING_H

#include <stdlib.h>

typedef struct {
    uint8_t* img;
    size_t width;
    size_t height;
} ImageSprite;

ImageSprite createSprite(uint8_t* img, int width, int height);
void destroySprite(ImageSprite* sprite);

#endif // BUILDING_H
