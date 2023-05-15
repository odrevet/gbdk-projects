#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdio.h>

#include "World1Tileset.h"
#include "world1area1.h"

#include "cursor.h"

#define TILE_SIZE 8

void display_map() {
  set_bkg_data(0, World1Tileset_TILE_COUNT, World1Tileset_tiles);
  set_bkg_tiles(0, 0, world1area1_WIDTH, world1area1_HEIGHT, world1area1_map);
}

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

  // joypad
  int joypad_previous, joypad_current;

  // cursor
  int cursor_x = 6, cursor_y = 6;
  set_sprite_data(0, 1, cursor_tiles);
  set_sprite_tile(0, 0);
  move_sprite(0, cursor_x * 8, cursor_y * 8);

  // map
  display_map();

  while (1) {
    joypad_previous = joypad_current;
    joypad_current = joypad();

    if (joypad_current & J_RIGHT && !(joypad_previous & J_RIGHT))
      cursor_x++;
    if (joypad_current & J_LEFT && !(joypad_previous & J_LEFT))
      cursor_x--;
    if (joypad_current & J_UP && !(joypad_previous & J_UP))
      cursor_y--;
    if (joypad_current & J_DOWN && !(joypad_previous & J_DOWN))
      cursor_y++;

    move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);

    wait_vbl_done();
  }
}
