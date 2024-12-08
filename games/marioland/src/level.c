#include "level.h"
#include <stddef.h>

uint16_t camera_x;
uint16_t camera_x_subpixel;
uint16_t next_col_chunk_load;
const unsigned char* current_map;
uint8_t map_buffer[LEVEL_HEIGHT][MAP_BUFFER_WIDTH];
uint8_t coldata[LEVEL_HEIGHT];
uint8_t set_column_at;
bool level_end_reached;

int current_map_tile_origin;
const unsigned char*  current_map_tiles_bin;
size_t current_map_size;
size_t current_map_width;


void load_current_level()
{
  set_column_at = 0;
  camera_x = 0;
  camera_x_subpixel = 0;
  level_end_reached = false;
  set_bkg_data(current_map_tile_origin, current_map_size, current_map_tiles_bin);
  rle_init(current_map);
  bkg_load_column(0, DEVICE_SCREEN_WIDTH + COLUMN_CHUNK_SIZE);
  next_col_chunk_load = COLUMN_CHUNK_SIZE;
}

void set_level_1()
{
  set_bkg_data(common_TILE_ORIGIN, INCBIN_SIZE(common_tiles_bin) >> 4, common_tiles_bin);
  current_map = map_1_1;
  current_map_tile_origin = birabuto_TILE_ORIGIN;
  current_map_tiles_bin = birabuto_tiles_bin;
  current_map_size = INCBIN_SIZE(birabuto_tiles_bin) >> 4;
  current_map_width = level_1_1_WIDTH;
}

void set_level_2()
{
  set_bkg_data(common_TILE_ORIGIN, INCBIN_SIZE(common_tiles_bin) >> 4, common_tiles_bin);
  current_map = map_1_2;
  current_map_tile_origin = birabuto_TILE_ORIGIN;
  current_map_tiles_bin = birabuto_tiles_bin;
  current_map_size = INCBIN_SIZE(birabuto_tiles_bin) >> 4;
  current_map_width = level_1_2_WIDTH;
}

