#include <stdbool.h>

#include <gbdk/incbin.h>
#include <gbdk/platform.h>
#include <gbdk/rledecompress.h>

#include "../res/level.h"

INCBIN(level_map_bin_rle, "res/level_map.bin.rle")
INCBIN_EXTERN(level_map_bin_rle)

INCBIN(level_tiles_bin, "res/level_tiles.bin")
INCBIN_EXTERN(level_tiles_bin)

#define MAP_DATA_HEIGHT 18
uint8_t data[MAP_DATA_HEIGHT];
uint8_t scrollpos = 0;
uint8_t datapos = 0;

uint8_t joy;

// current and old positions of the camera in pixels
uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
// current and old position of the map in tiles
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;

#define camera_max_x level_WIDTH

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;

  // joypad
  int joypad_previous, joypad_current = 0;

  // map
  rle_init(level_map_bin_rle);
  set_bkg_data(0, INCBIN_SIZE(level_tiles_bin) >> 4, level_tiles_bin);

  for (uint8_t i = 0; (i != DEVICE_SCREEN_WIDTH + 1); i++) {
    rle_decompress(data, MAP_DATA_HEIGHT);
    set_bkg_tiles(i & (DEVICE_SCREEN_BUFFER_WIDTH - 1), 0, 1, MAP_DATA_HEIGHT,
                  data);
  }

  datapos = 0;
  scrollpos = 1;

  while (1) {
    joypad_previous = joypad_current;
    joypad_current = joypad();
    if (joypad_current & J_RIGHT) {
      if (camera_x < camera_max_x) {
        scrollpos++;
        move_bkg(scrollpos, 0);
      }
    }

    vsync();

    if ((scrollpos & 0x07u) == 0) {

      datapos = (scrollpos >> 3);
      uint8_t map_x_column =
          (datapos + DEVICE_SCREEN_WIDTH) & (DEVICE_SCREEN_BUFFER_WIDTH - 1);

      if (!rle_decompress(data, MAP_DATA_HEIGHT)) {
        rle_init(level_map_bin_rle);
        rle_decompress(data, MAP_DATA_HEIGHT);
      }

      set_bkg_tiles(map_x_column, 0, 1, MAP_DATA_HEIGHT, data);
    }
  }
}
