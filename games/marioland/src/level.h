#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <gb/gb.h>
#include <gb/metasprites.h>
#include <gbdk/incbin.h>
#include <gbdk/platform.h>

#include "global.h"

// tilesets
#include "graphics/common.h"
BANKREF_EXTERN(common)

#include "graphics/birabuto.h"
BANKREF_EXTERN(birabuto)

// maps
#include "levels/level_1_1.h"
BANKREF_EXTERN(level_1_1)

//#include "levels/level_1_2.h"
//BANKREF_EXTERN(level_1_2)

//#include "levels/level_1_3.h"
//BANKREF_EXTERN(level_1_3)

// buffer worth of one column to hold map data when reading
extern uint8_t coldata[LEVEL_HEIGHT];
// map buffer in RAM to check collision without access VRAM
#define MAP_BUFFER_WIDTH (DEVICE_SCREEN_WIDTH + COLUMN_CHUNK_SIZE)
#define MAP_BUFFER_HEIGHT (LEVEL_HEIGHT)
#define MAP_BUFFER_SIZE (MAP_BUFFER_WIDTH * MAP_BUFFER_HEIGHT)
#define MAX_TILE 255

extern uint8_t map_buffer[MAP_BUFFER_SIZE];

extern uint16_t camera_x;
extern uint16_t camera_x_subpixel;
extern uint16_t next_col_chunk_load;
extern uint8_t set_column_at;
extern bool level_end_reached;
extern uint8_t current_level;

extern const unsigned char* current_map_data;
extern const unsigned char* current_map_tiles;
extern int current_map_tile_origin;
extern size_t current_map_width;


enum tileset_index {
  TILE_EMPTY = 0x27,
  TILE_INTEROGATION_BLOCK = 0x28,
  TILE_EMPTIED = 0x29,
  BREAKABLE_BLOCK = 0x2A,
  TILE_UNBREAKABLE = 0x2B,
  TILE_COIN = 0x33,
  PIPE_TOP_LEFT = 0x30,
  PIPE_TOP_RIGHT = 0x31,
  PIPE_CENTER_LEFT = 0x3A,
  PIPE_CENTER_RIGHT = 0x3B,
  TILE_FLOOR = 0x65,
  TILE_METALIC_LEFT = 0x4A,
  TILE_METALIC_RIGHT = 0x4B,
  PALM_PLATEFORM_LEFT = 0x69,
  PALM_PLATEFORM_CENTER = 0X6A,
  PALM_PLATEFORM_RIGHT = 0x6B,
  STONE_TILE_LEFT = 0x7F,
  STONE_TILE_MIDDLE_LEFT = 0x89,
  STONE_TILE_MIDDLE_RIGHT = 0x8A,
  STONE_TILE_RIGHT = 0x80,
  STONE_BAR = 0x81,
  STONE_TILE_FLOOR = 0x8B,
};

uint8_t level_get_tile(uint8_t x, uint8_t y) NONBANKED;
bool level_is_tile_solid(uint8_t tile) NONBANKED;
uint8_t level_load_column(uint8_t start_at, uint8_t nb) NONBANKED;

void level_set_tileset_birabuto();
void level_set_current() NONBANKED;
void level_load_current();
void level_set_1_1() ;
void level_set_1_2() ;
void level_set_1_3() ;

#endif