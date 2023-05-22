#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/World1Tileset.h"
#include "graphics/cursor.h"
#include "maps/world1area1.h"

#include "text.h"

#define TILE_SIZE 8
#define OFFSET_X 1
#define OFFSET_Y 2
#define SCREEN_HEIGHT 18
#define SCREEN_WIDTH 20

void main(void) {
  DISPLAY_ON;
  SHOW_WIN;
  SHOW_BKG;
  SHOW_SPRITES;

  // text
  text_load_font();
  move_win(7, 128);
  char buffer[SCREEN_WIDTH * 2];
  unsigned char windata[SCREEN_WIDTH * 2];
  memset(windata, 15, SCREEN_WIDTH * 2);

  // joypad
  int joypad_previous, joypad_current;

  // cursor
  int cursor_x = (SCREEN_WIDTH / 2), cursor_y = (SCREEN_HEIGHT / 2);
  set_sprite_data(0, 1, cursor_tiles);
  set_sprite_tile(0, 0);
  move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);

  // map
  set_bkg_data(0, World1Tileset_TILE_COUNT, World1Tileset_tiles);
  set_bkg_tiles(0, 0, world1area1_WIDTH, world1area1_HEIGHT, world1area1_map);

  while (1) {
    set_win_tiles(0, 0, SCREEN_WIDTH, 2, windata);
    text_print_string_win(0, 0, buffer);

    joypad_previous = joypad_current;
    joypad_current = joypad();

    if (joypad_current & J_RIGHT && !(joypad_previous & J_RIGHT) &&
        cursor_x < world1area1_WIDTH)
      cursor_x++;
    if (joypad_current & J_LEFT && !(joypad_previous & J_LEFT) && cursor_x > 1)
      cursor_x--;
    if (joypad_current & J_UP && !(joypad_previous & J_UP) && cursor_y > 2)
      cursor_y--;
    if (joypad_current & J_DOWN && !(joypad_previous & J_DOWN) &&
        cursor_y <= world1area1_HEIGHT)
      cursor_y++;

    if (joypad_current != joypad_previous) {
      move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);
    }

    char fmt[] = "X:%d Y:%d INDEX:%d\nTILE:%d ATTR:%d";
    int index = (cursor_y - 2) * world1area1_WIDTH + (cursor_x - 1);
    sprintf(buffer, fmt, (int16_t)cursor_x - OFFSET_X,
            (int16_t)cursor_y - OFFSET_Y, (int16_t)index,
            world1area1_map[world1area1_WIDTH * (cursor_y - OFFSET_Y) +
                            (cursor_x - OFFSET_X)],
            (int16_t)world1area1_map_attributes[index]);

    wait_vbl_done();
  }
}
