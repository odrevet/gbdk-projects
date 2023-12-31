#include <gb/gb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/cursor.h"
#include "level.h"
#include "text.h"

#define TILE_SIZE 8
#define WINDOW_HEIGHT 2
#define WINDOW_WIDTH DEVICE_SCREEN_WIDTH
#define WINDOW_SIZE WINDOW_WIDTH *WINDOW_HEIGHT
#define WINDOW_X 7
#define WINDOW_Y 0

#define camera_max_y ((level_HEIGHT - DEVICE_SCREEN_HEIGHT) * TILE_SIZE)
#define camera_max_x ((level_WIDTH - DEVICE_SCREEN_WIDTH) * TILE_SIZE)

#define MIN(A, B) ((A) < (B) ? (A) : (B))

uint8_t joy;

// current and old positions of the camera in pixels
uint16_t camera_x, camera_y, old_camera_x, old_camera_y;

// current and old position of the map in tiles
uint8_t map_pos_x, map_pos_y, old_map_pos_x, old_map_pos_y;

// redraw flag, indicates that camera position was changed
uint8_t redraw;

bool move_camera;
char fmt[] = "X:%d Y:%d INDEX:%d\nROM:%d VRAM: %d";

void set_camera() {
  // update hardware scroll position
  SCY_REG = camera_y;
  SCX_REG = camera_x;
  // up or down
  map_pos_y = (uint8_t)(camera_y >> 3u);
  if (map_pos_y != old_map_pos_y) {
    if (camera_y < old_camera_y) {
      set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, level_WIDTH - map_pos_x), 1,
                     level_map, level_WIDTH);
    } else {
      if ((level_HEIGHT - 18u) > map_pos_y)
        set_bkg_submap(map_pos_x, map_pos_y + 18u,
                       MIN(21u, level_WIDTH - map_pos_x), 1, level_map,
                       level_WIDTH);
    }
    old_map_pos_y = map_pos_y;
  }
  // left or right
  map_pos_x = (uint8_t)(camera_x >> 3u);
  if (map_pos_x != old_map_pos_x) {
    if (camera_x < old_camera_x) {
      set_bkg_submap(map_pos_x, map_pos_y, 1,
                     MIN(19u, level_HEIGHT - map_pos_y), level_map,
                     level_WIDTH / 8);
    } else {
      if ((level_WIDTH - 20u) > map_pos_x)
        set_bkg_submap(map_pos_x + 20u, map_pos_y, 1,
                       MIN(19u, level_HEIGHT - map_pos_y), level_map,
                       level_WIDTH / 8);
    }
    old_map_pos_x = map_pos_x;
  }
  // set old camera position to current camera position
  old_camera_x = camera_x, old_camera_y = camera_y;
}

void interupt() {
  if (!move_camera) {
    int h = WINDOW_Y + WINDOW_HEIGHT * TILE_SIZE - 1;
    if (LYC_REG == WINDOW_Y) {
      SHOW_WIN;
      LYC_REG = h;
    } else if (h) {
      HIDE_WIN;
      LYC_REG = WINDOW_Y;
    }
  }
}

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;

  STAT_REG |= 0x40U;
  LYC_REG = WINDOW_Y;
  disable_interrupts();
  add_LCD(interupt);
  set_interrupts(LCD_IFLAG | VBL_IFLAG);
  enable_interrupts();

  move_camera = TRUE;

  // text
  text_load_font();

  // joypad
  int joypad_previous, joypad_current = 0;

  // cursor
  int cursor_x = (DEVICE_SCREEN_WIDTH / 2),
      cursor_y = (DEVICE_SCREEN_HEIGHT / 2);
  set_sprite_data(0, 1, cursor_tiles);
  set_sprite_tile(0, 0);

  // map
  map_pos_x = map_pos_y = 0;
  old_map_pos_x = old_map_pos_y = 255;
  set_bkg_submap(map_pos_x, map_pos_y, DEVICE_SCREEN_WIDTH,
                 DEVICE_SCREEN_HEIGHT, level_map, level_WIDTH / TILE_SIZE);

  camera_x = camera_y = 0;
  old_camera_x = camera_x;
  old_camera_y = camera_y;

  redraw = FALSE;

  SCX_REG = camera_x;
  SCY_REG = camera_y;

  set_bkg_data(0, level_TILE_COUNT, level_tiles);

  // text
  char buffer[WINDOW_SIZE];
  unsigned char windata[WINDOW_SIZE];
  memset(windata, 15, WINDOW_SIZE);
  set_win_tiles(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, windata);
  move_win(WINDOW_X, WINDOW_Y);

  while (1) {
    joypad_previous = joypad_current;
    joypad_current = joypad();

    if (joypad_current & J_A && !(joypad_previous & J_A)) {
      move_camera = !move_camera;
      if (move_camera) {
        HIDE_WIN;
        HIDE_SPRITES;
      } else {
        SHOW_WIN;
        SHOW_SPRITES;
      }
    }

    if (move_camera) {
      if (joypad_current & J_RIGHT &&
          camera_x + DEVICE_SCREEN_PX_WIDTH < level_WIDTH) {
        if (camera_x < camera_max_x) {
          camera_x++;
          redraw = TRUE;
        }
      }
      if (joypad_current & J_LEFT && camera_x > 0) {
        if (camera_x)
          camera_x--;
        redraw = TRUE;
      }
      if (joypad_current & J_UP && camera_y < 0) {
        if (camera_y) {
          camera_y--;
          redraw = TRUE;
        }
      }
      if (joypad_current & J_DOWN &&
          camera_y + DEVICE_SCREEN_PX_HEIGHT < level_HEIGHT) {
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

      // text
      int16_t cursor_x_shifted =
          cursor_x - DEVICE_SPRITE_PX_OFFSET_X / TILE_SIZE;
      int16_t cursor_y_shifted =
          cursor_y - DEVICE_SPRITE_PX_OFFSET_Y / TILE_SIZE;
      int16_t index =
          cursor_y_shifted * (level_WIDTH / TILE_SIZE) + cursor_x_shifted;
      sprintf(buffer, fmt, (int16_t)cursor_x_shifted, (int16_t)cursor_y_shifted,
              (int16_t)index, level_map[index],
              (uint8_t)get_bkg_tile_xy(cursor_x_shifted, cursor_y_shifted));
      text_print_string_win(0, 0, buffer);
    }

    move_sprite(0, cursor_x * TILE_SIZE - camera_x,
                cursor_y * TILE_SIZE - camera_y);

    vsync();

    if (move_camera && redraw) {
      set_camera();
      redraw = FALSE;
    }
  }
}
