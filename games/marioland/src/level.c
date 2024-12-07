#include "level.h"
#include <stddef.h>

uint16_t camera_x;
uint16_t camera_x_subpixel;
uint16_t next_col_chunk_load;
const unsigned char* current_map;
uint8_t map_buffer[LEVEL_HEIGHT][MAP_BUFFER_WIDTH];
uint8_t coldata[LEVEL_HEIGHT];
uint8_t set_column_at;

int current_map_tile_origin;
const unsigned char*  current_map_tiles_bin;
size_t current_map_size;
size_t current_map_width;


void init_level()
{
  set_column_at = 0;
  camera_x = 0;
  camera_x_subpixel = 0;
  set_bkg_data(current_map_tile_origin, current_map_size, current_map_tiles_bin);
  rle_init(current_map);
  bkg_load_column(0, DEVICE_SCREEN_WIDTH + COLUMN_CHUNK_SIZE);
  next_col_chunk_load = COLUMN_CHUNK_SIZE;
}
