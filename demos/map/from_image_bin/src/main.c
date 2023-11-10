#include <gbdk/platform.h>
#include <stdint.h>

#include <gbdk/incbin.h>

INCBIN(level_map_bin, "res/level_map.bin")
INCBIN_EXTERN(level_map_bin)

INCBIN(level_tiles_bin, "res/level_tiles.bin")
INCBIN_EXTERN(level_tiles_bin)

#define TILE_BYTES 16

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  set_bkg_data(0, INCBIN_SIZE(level_tiles_bin) / TILE_BYTES, level_tiles_bin);
  set_bkg_tiles(0, 0, 20, 18, level_map_bin);
}