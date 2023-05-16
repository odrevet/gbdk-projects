#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "World1Tileset.h"
#include "world1area1.h"

#include "cursor.h"
#include "text.h"

#define TILE_SIZE 8
#define OFFSET_X 1
#define OFFSET_Y 2
#define SCREEN_HEIGHT 18
#define SCREEN_WIDTH 20

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

  // text
  text_load_font();

  // joypad
  int joypad_previous, joypad_current;

  // cursor
  int cursor_x = 6 - OFFSET_X, cursor_y = 6 - OFFSET_Y;
  set_sprite_data(0, 1, cursor_tiles);
  set_sprite_tile(0, 0);
  move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);

  // map
  set_bkg_data(0, World1Tileset_TILE_COUNT, World1Tileset_tiles);
  set_bkg_tiles(0, 0, world1area1_WIDTH, world1area1_HEIGHT, world1area1_map);

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

    if (joypad_current != joypad_previous) {
      move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);
      set_bkg_tiles(0, 0, world1area1_WIDTH, world1area1_HEIGHT,
                    world1area1_map);
      // text
      char buffer[SCREEN_WIDTH * 2];
      int index = (cursor_y - 2) * world1area1_WIDTH + (cursor_x - 1);
      sprintf(buffer, "X:%d Y:%d INDEX:%d\nTILE:%d ATTR:%d",
              (int16_t)cursor_x - 1, (int16_t)cursor_y - 2, (int16_t)index,
              world1area1_map[world1area1_WIDTH * cursor_y + cursor_x],
              (int16_t)world1area1_map_attributes[index]);
      text_print_string_bkg(0, 0, buffer);
    }

    wait_vbl_done();
  }
}
