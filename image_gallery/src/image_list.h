#ifndef IMAGE_LIST
#define IMAGE_LIST

#define NB_IMAGES 4

#include "image_0.h"
#include "image_1.h"
#include "image_2.h"
#include "image_3.h"

const uint8_t TILE_COUNT[] = {image_0_TILE_COUNT, image_1_TILE_COUNT, image_2_TILE_COUNT, image_3_TILE_COUNT};
const uint8_t *TILES[] = {image_0_tiles, image_1_tiles, image_2_tiles, image_3_tiles};
const unsigned char *MAP[] = {image_0_map, image_1_map, image_2_map, image_3_map};

#endif
