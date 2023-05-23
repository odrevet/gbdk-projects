#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/World1Tileset.h"
#include "graphics/cursor.h"
#include "maps/world1area1.h"
#include "maps/world1area2.h"

#include "text.h"

#define TILE_SIZE 8
#define OFFSET_X 1
#define OFFSET_Y 2
#define SCREEN_HEIGHT 18
#define SCREEN_WIDTH 20
#define WINDOW_HEIGHT 2
#define WINDOW_WIDTH SCREEN_WIDTH
#define WINDOW_SIZE WINDOW_WIDTH *WINDOW_HEIGHT

#define camera_max_y ((map_height - 18) * 8)
#define camera_max_x ((map_width - 20) * 8)

#define MIN(A, B) ((A) < (B) ? (A) : (B))

uint8_t joy;

// current and old positions of the camera in pixels
uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
// current and old position of the map in tiles
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;
// redraw flag, indicates that camera position was changed
uint8_t redraw;

// map
const unsigned char *map;
const unsigned char *map_attributes;
short map_width;
short map_height;
short map_tile_count;

void set_camera() {
  // update hardware scroll position
  SCY_REG = camera_y;
  SCX_REG = camera_x;
  // up or down
  map_pos_y = (uint8_t)(camera_y >> 3u);
  if (map_pos_y != old_map_pos_y) {
    if (camera_y < old_camera_y) {
      set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, map_width - map_pos_x), 1,
                     map, map_width);
    } else {
      if ((map_height - 18u) > map_pos_y)
        set_bkg_submap(map_pos_x, map_pos_y + 18u,
                       MIN(21u, map_width - map_pos_x), 1, map, map_width);
    }
    old_map_pos_y = map_pos_y;
  }
  // left or right
  map_pos_x = (uint8_t)(camera_x >> 3u);
  if (map_pos_x != old_map_pos_x) {
    if (camera_x < old_camera_x) {
      set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, map_height - map_pos_y),
                     map, map_width);
    } else {
      if ((map_width - 20u) > map_pos_x)
        set_bkg_submap(map_pos_x + 20u, map_pos_y, 1,
                       MIN(19u, map_height - map_pos_y), map, map_width);
    }
    old_map_pos_x = map_pos_x;
  }
  // set old camera position to current camera position
  old_camera_x = camera_x, old_camera_y = camera_y;
}

void init_map() {
  map_pos_x = map_pos_y = 0;
  old_map_pos_x = old_map_pos_y = 255;
  set_bkg_submap(map_pos_x, map_pos_y, 20, 18, map, map_width);

  camera_x = camera_y = 0;
  old_camera_x = camera_x;
  old_camera_y = camera_y;

  redraw = FALSE;

  SCX_REG = camera_x;
  SCY_REG = camera_y;
}

void load_area1() {
  map = world1area1_map;
  map_attributes = world1area1_map_attributes;
  map_width = world1area1_WIDTH;
  map_height = world1area1_HEIGHT;
  map_tile_count = world1area1_TILE_COUNT;
}

void load_area2() {
  map = world1area2_map;
  map_attributes = world1area2_map_attributes;
  map_width = world1area2_WIDTH;
  map_height = world1area2_HEIGHT;
  map_tile_count = world1area2_TILE_COUNT;
}

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  SHOW_SPRITES;

  bool move_camera = TRUE;

  // text
  text_load_font();

  // joypad
  int joypad_previous, joypad_current = 0;

  // cursor
  int cursor_x = (SCREEN_WIDTH / 2), cursor_y = (SCREEN_HEIGHT / 2);
  set_sprite_data(0, 1, cursor_tiles);
  set_sprite_tile(0, 0);
  move_sprite(0, cursor_x * TILE_SIZE, cursor_y * TILE_SIZE);

  short level_index = 0;
  load_area1();
  init_map();

  set_bkg_data(0, World1Tileset_TILE_COUNT, World1Tileset_tiles);

  // text
  char buffer[WINDOW_SIZE];
  unsigned char windata[WINDOW_SIZE];
  memset(windata, 15, WINDOW_SIZE);
  set_win_tiles(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windata);
  move_win(7, 128);

  while (1) {
    joypad_previous = joypad_current;
    joypad_current = joypad();

    if (joypad_current & J_A && !(joypad_previous & J_A)) {
      move_camera = !move_camera;
      if (move_camera)
        HIDE_WIN;
      else
        SHOW_WIN;
    }

    if (joypad_current & J_SELECT && !(joypad_previous & J_SELECT)) {
      level_index++;
      if (level_index == 2) {
        level_index = 0;
      }

      if (level_index == 0) {
        load_area1();
      } else {
        load_area2();
      }

      init_map();
    }

    if (move_camera) {
      if (joypad_current & J_RIGHT) {
        if (camera_x < camera_max_x) {
          camera_x++;
          redraw = TRUE;
        }
      }
      if (joypad_current & J_LEFT) {
        if (camera_x)
          camera_x--;
        redraw = TRUE;
      }
      if (joypad_current & J_UP) {
        if (camera_y) {
          camera_y--;
          redraw = TRUE;
        }
      }
      if (joypad_current & J_DOWN) {
        if (camera_y < camera_max_y) {
          camera_y++;
          redraw = TRUE;
        }
      }
    } else {
      if (joypad_current & J_RIGHT && !(joypad_previous & J_RIGHT))
        cursor_x++;
      if (joypad_current & J_LEFT && !(joypad_previous & J_LEFT))
        cursor_x--;
      if (joypad_current & J_UP && !(joypad_previous & J_UP))
        cursor_y--;
      if (joypad_current & J_DOWN && !(joypad_previous & J_DOWN))
        cursor_y++;

      // print text
      char fmt[] = "X:%d Y:%d INDEX:%d\nTILE:%d ATTR:%d";
      int index = (cursor_y - 2) * map_width + (cursor_x - 1);
      sprintf(buffer, fmt, (int16_t)cursor_x - OFFSET_X,
              (int16_t)cursor_y - OFFSET_Y, (int16_t)index,
              map[map_width * (cursor_y - OFFSET_Y) + (cursor_x - OFFSET_X)],
              (int16_t)map_attributes[index]);
      text_print_string_win(0, 0, buffer);
    }

    move_sprite(0, cursor_x * TILE_SIZE - camera_x,
                cursor_y * TILE_SIZE - camera_y);

    wait_vbl_done();
    if (move_camera && redraw) {
      set_camera();
      redraw = FALSE;
    }
  }
}
