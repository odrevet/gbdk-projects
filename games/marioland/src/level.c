#include "level.h"
#include "graphics/birabuto.h"
#include "levels/level_1_1.h"
#include <stddef.h>

uint16_t camera_x;
uint16_t camera_x_subpixel;
uint16_t next_col_chunk_load;

uint8_t map_buffer[MAP_BUFFER_SIZE];
uint8_t coldata[LEVEL_HEIGHT];
uint8_t set_column_at;
bool level_end_reached;
uint8_t current_level;


int current_map_tile_origin;
const unsigned char*  current_map_data;
const unsigned char*  current_map_tiles;
size_t current_map_size;
size_t current_map_width;

bool level_is_tile_solid(uint8_t tile) NONBANKED {
    static const bool solid_tiles[MAX_TILE + 1] = {
        [TILE_FLOOR] = true,
        [TILE_INTEROGATION_BLOCK] = true,
        [BREAKABLE_BLOCK] = true,
        [TILE_UNBREAKABLE] = true,
        [PIPE_TOP_LEFT] = true,
        [PIPE_TOP_RIGHT] = true,
        [PIPE_CENTER_LEFT] = true,
        [PIPE_CENTER_RIGHT] = true,
        [TILE_METALIC_LEFT] = true,
        [TILE_METALIC_RIGHT] = true,
        [TILE_EMPTIED] = true,
        [PALM_PLATEFORM_LEFT] = true,
        [PALM_PLATEFORM_CENTER] = true,
        [PALM_PLATEFORM_RIGHT] = true,
        [STONE_TILE_LEFT] = true,
        [STONE_TILE_RIGHT] = true,
        [STONE_BAR] = true,
        [STONE_TILE_FLOOR] = true
    };
    return tile <= MAX_TILE && solid_tiles[tile];
}

uint8_t level_load_column(uint8_t start_at, uint8_t nb) NONBANKED
{
  uint8_t col = 0;

  while (col < nb) {
    // Copy column from current_map_data to coldata
    for (uint8_t row = 0; row < LEVEL_HEIGHT; row++) {
      uint16_t index = (row * MAP_BUFFER_WIDTH) + set_column_at;
      coldata[row] = current_map_data[index];
    }

    set_column_at = (set_column_at + 1) % MAP_BUFFER_WIDTH;

    // Get hardware map tile X column
    uint8_t map_x_column = (col + start_at) & (DEVICE_SCREEN_BUFFER_WIDTH - 1);

    // Draw current column
    set_bkg_tiles(map_x_column, 0, 1, LEVEL_HEIGHT, coldata);

    col++;
  }

  return col;
}

uint8_t level_get_tile(uint8_t x, uint8_t y) NONBANKED {
  uint16_t index = ((y / TILE_SIZE - DEVICE_SPRITE_OFFSET_Y) * MAP_BUFFER_WIDTH) + 
                   (((x + camera_x) / TILE_SIZE) % MAP_BUFFER_WIDTH);
  return map_buffer[index];
}

void level_set_current() NONBANKED
{
  uint8_t _current_bank = CURRENT_BANK;

  switch(current_level){
    case 0: 
      SWITCH_ROM(( BANK(level_1_1)));
      level_set_1_1();
      break;
    case 1: 
      //SWITCH_ROM(( BANK(level_1_2)));
      level_set_1_2();
      break;
    case 2: 
      //SWITCH_ROM(( BANK(level_1_3)));
      level_set_1_3();
      break;
  }

  SWITCH_ROM(_current_bank);

  
}

void level_load_current() 
{
  set_column_at = 0;
  camera_x = 0;
  camera_x_subpixel = 0;
  level_end_reached = false;
  set_bkg_data(current_map_tile_origin, current_map_size, current_map_tiles);
  level_load_column(0, DEVICE_SCREEN_WIDTH + COLUMN_CHUNK_SIZE);
  next_col_chunk_load = COLUMN_CHUNK_SIZE;
}

void level_set_tileset_birabuto()
{
  current_map_tiles = birabuto_tiles;
  current_map_tile_origin = birabuto_TILE_ORIGIN;
  current_map_size = birabuto_TILE_COUNT;
}

void level_set_1_1() 
{
  level_set_tileset_birabuto();
  current_map_data = level_1_1_map;
  current_map_width = level_1_1_WIDTH;
}

void level_set_1_2() 
{
  level_set_tileset_birabuto();
  //current_map_data = level_1_2_map;
  //current_map_width = level_1_2_WIDTH;
}

void level_set_1_3() 
{
  level_set_tileset_birabuto();
  //current_map_data = level_1_3_map;
  //current_map_width = level_1_3_WIDTH;
}

