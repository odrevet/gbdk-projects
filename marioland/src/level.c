#include "level.h"

const unsigned char *map;
const unsigned char *map_attributes;
short map_width;
short map_height;
short map_tile_count;

bool is_solid(int x, int y) {
  const unsigned char tile =
      map[map_width * (y / TILE_SIZE - 1) + (x / TILE_SIZE)];
  return (tile != 0xf                   // empty
          && tile != 0x1 && tile != 0xc // sky
          && tile != 11                 // coin
  );
}
