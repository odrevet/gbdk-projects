#include <gb/gb.h>
#include <gb/metasprites.h>

#include "World1Tileset.h"

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  set_bkg_data(0, 28, World1Tileset_tiles);
  set_bkg_tiles(0, 0, 20, 18, World1Tileset_map);
}