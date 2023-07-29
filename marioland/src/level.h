#ifndef LEVEL_H
#define LEVEL_H

#include <gb/gb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "camera.h"
#include "global.h"

extern const unsigned char *map;
extern const unsigned char *map_attributes;
extern short map_width;
extern short map_height;
extern short map_tile_count;


bool is_solid(int x, int y);

inline bool is_coin(int x, int y) {
  return get_bkg_tile_xy(x / TILE_SIZE, y / TILE_SIZE) == 11;
}

#endif