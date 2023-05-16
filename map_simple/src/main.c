#include <gb/gb.h>
#include <stdbool.h>
#include <stdint.h>

#include "level_bg.h"
#include "map.h"

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  set_bkg_data(0, 4, level_bg);
  set_bkg_tiles(0, 0, mapWidth, mapHeight, map);
}