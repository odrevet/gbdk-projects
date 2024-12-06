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
#include <gbdk/rledecompress.h>

#include "global.h"

#define LEVEL_HEIGHT 16
#define COLUMN_CHUNK_SIZE 3 // how many map columns to decompress at a time

// buffer worth of one column to hold map data when decrompressing
extern uint8_t coldata[LEVEL_HEIGHT];
// map buffer in RAM to check collision without access VRAM
#define MAP_BUFFER_WIDTH (DEVICE_SCREEN_WIDTH + COLUMN_CHUNK_SIZE)
extern uint8_t map_buffer[LEVEL_HEIGHT][MAP_BUFFER_WIDTH];

extern uint16_t camera_x;
extern uint16_t camera_x_subpixel;
extern uint16_t next_col_chunk_load;
extern const unsigned char* current_map;
extern uint8_t set_column_at;
extern int current_map_tile_origin;
extern const unsigned char*  current_map_tiles_bin;
extern size_t current_map_size;

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
  TILE_METALIC_RIGHT = 0x4B
};

inline uint8_t get_tile(uint8_t x, uint8_t y) {
  return map_buffer[y / TILE_SIZE - DEVICE_SPRITE_OFFSET_Y]
                   [((x + camera_x) / TILE_SIZE) % MAP_BUFFER_WIDTH];
}

inline bool is_tile_solid(uint8_t tile) {
  return tile == TILE_FLOOR || tile == TILE_INTEROGATION_BLOCK ||
         tile == BREAKABLE_BLOCK || tile == TILE_UNBREAKABLE ||
         tile == PIPE_TOP_LEFT || tile == PIPE_TOP_RIGHT ||
         tile == PIPE_CENTER_LEFT || tile == PIPE_CENTER_RIGHT ||
         tile == TILE_METALIC_LEFT || tile == TILE_METALIC_RIGHT ||
         tile == TILE_EMPTIED;
}


inline uint8_t bkg_load_column(uint8_t start_at, uint8_t nb) {
  uint8_t col = 0;
  while (col < nb && rle_decompress(coldata, LEVEL_HEIGHT)) {
    // copy column to map_buffer
    for (uint8_t row = 0; row < LEVEL_HEIGHT; row++) {
      map_buffer[row][set_column_at] = coldata[row];
    }

    set_column_at = ++set_column_at % MAP_BUFFER_WIDTH;

    // Get hardware map tile X column
    uint8_t map_x_column = (col + start_at) & (DEVICE_SCREEN_BUFFER_WIDTH - 1);

    // Draw current column
    set_bkg_tiles(map_x_column, 0, 1, LEVEL_HEIGHT, coldata);

    col++;
  }

  return col;
}

void init_level();

#endif