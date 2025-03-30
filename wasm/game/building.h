#ifndef BUILDING_H
#define BUILDING_H

#include <stdlib.h>
#include <coord.h>

typedef struct {
    uint8_t* img;
    size_t width;
    size_t height;
} ImageSprite;

typedef struct {
    ImageSprite* img;
    Pos pos;
} Building;

ImageSprite* createSprite(uint8_t* img, int width, int height);
void destroySprite(ImageSprite* sprite);

#endif // BUILDING_H
