#include <gb/gb.h>
#include <gb/metasprites.h>

#include "world1area1.h"
#include "World1Tileset.h"

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  set_bkg_data(1, World1Tileset_TILE_COUNT, World1Tileset_tiles);
  set_bkg_tiles(0, 0, world1area1_WIDTH, world1area1_HEIGHT, world1area1_map);
}