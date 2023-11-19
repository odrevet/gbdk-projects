#include <gb/gb.h>

#include "level.h"

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  set_bkg_data(0, level_TILE_COUNT, level_tiles);
  set_bkg_tiles(0, 0, 20, 18, level_map);
}
