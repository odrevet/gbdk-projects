#include "level.h"

const unsigned char *map;
const unsigned char *map_attributes;
short map_width;
short map_height;
short map_tile_count;

bool is_solid(int x, int y) {
  const unsigned char tile = get_bkg_tile_xy(x / TILE_SIZE, y / TILE_SIZE - 2);
  return tile == 0x22;
}
