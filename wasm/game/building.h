#ifndef BUILDING_H
#define BUILDING_H

#include <stdlib.h>
#include <coord.h>
#include <common.h>

typedef struct {
    uint8_t* img;
    size_t width;
    size_t height;
} ImageSprite;

typedef struct {
    ImageSprite* sprites[MAX_SCALE-MIN_SCALE+1];
    Pos pos;
} Building;

ImageSprite* createSprite(uint8_t* img, int width, int height);
void destroySprite(ImageSprite* sprite);

ImageSprite* getImgForScale(Building* building, int scale);

#endif // BUILDING_H
