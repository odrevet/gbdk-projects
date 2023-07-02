#include <gb/gb.h>
#include <gb/metasprites.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/World1Tileset.h"
#include "graphics/mario.h"
#include "maps/world1area1.h"
#include "maps/world1area2.h"

#include "camera.h"
#include "hUGEDriver.h"
#include "sound.h"
#include "text.h"

#define TILE_SIZE 8
#define OFFSET_X 1
#define OFFSET_Y 2
#define SCREEN_HEIGHT_TILE 18
#define SCREEN_WIDTH_TILE 20
#define WINDOW_HEIGHT_TILE 2
#define WINDOW_WIDTH_TILE SCREEN_WIDTH_TILE
#define WINDOW_SIZE WINDOW_WIDTH_TILE *WINDOW_HEIGHT_TILE
#define WINDOW_X MINWNDPOSX
#define WINDOW_Y MINWNDPOSY

#define TIME_INITIAL_VALUE 400
#define GRAVITY_SPEED 36
#define JUMP_SPEED 25
#define MARIO_SPEED_WALK 20
#define MARIO_SPEED_RUN 28
#define JUMP_MAX (5 * TILE_SIZE / JUMP_SPEED) << 4
#define LOOP_PER_ANIMATION_FRAME 5
#define MARIO_HALF_WIDTH TILE_SIZE / 2

const uint8_t window_location = WINDOW_Y + WINDOW_HEIGHT_TILE * TILE_SIZE;

uint8_t coins;
uint16_t score;
bool redraw;
uint8_t joy;
uint16_t time;
uint8_t lives;
uint8_t level_index;

// map
const unsigned char *map;
const unsigned char *map_attributes;
short map_width;
short map_height;
short map_tile_count;

// player coords and movements
uint16_t player_x;
uint16_t player_y;
uint16_t player_x_next;
uint16_t player_y_next;
uint16_t player_draw_x;
uint16_t player_draw_y;
uint16_t player_draw_x_next;
uint16_t player_draw_y_next;
int8_t vel_x;
int8_t vel_y;
bool is_jumping = FALSE;
bool display_jump_frame;
bool touch_ground = FALSE;
uint8_t current_jump = 0;
uint8_t mario_speed = 0;
uint8_t mario_current_frame = 0;
uint8_t frame_counter = 0;

// music
// extern const hUGESong_t fish_n_chips;
extern const hUGESong_t cognition;

void update_frame_counter() {
  frame_counter++;
  if (frame_counter == LOOP_PER_ANIMATION_FRAME) {
    frame_counter = 0;
    mario_current_frame = (mario_current_frame % 3) + 1;
  }
}

void init_map() {
  map_pos_x = 0;
  old_map_pos_x = 255;
  set_bkg_submap(map_pos_x, 0, 20, 18, map, map_width);

  camera_x = 0;
  old_camera_x = camera_x;

  redraw = FALSE;

  SCX_REG = camera_x;
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

void interupt() {
  if (LYC_REG == WINDOW_Y) {
    SHOW_WIN;
    LYC_REG = window_location;
  } else if (window_location) {
    HIDE_WIN;
    LYC_REG = WINDOW_Y;
  }
}

// TODO use solid map when available
bool is_solid(int x, int y) {
  const unsigned char tile =
      map[map_width * (y / TILE_SIZE - 1) + (x / TILE_SIZE)];
  return (tile != 0xf                   // empty
          && tile != 0x1 && tile != 0xc // sky
          && tile != 11                 // coin
  );
}

bool is_coin(int x, int y) {
  return get_bkg_tile_xy(x / TILE_SIZE, y / TILE_SIZE) == 11;
}

void hud_update_coins() {
  char coins_str[3];

  if (coins < 10) {
    coins_str[0] = '0'; // Add leading zero
    itoa(coins, coins_str + 1, 10);
  } else {
    itoa(coins, coins_str, 10);
  }

  text_print_string_win(9, 1, coins_str);
}

void hud_update_score() {
  char score_str[4];
  itoa(score, score_str, 10);
  text_print_string_win(0, 1, score_str);
}

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  SPRITES_8x16;

  STAT_REG |= 0x40U;
  LYC_REG = WINDOW_Y;
  disable_interrupts();
  add_LCD(interupt);
  set_interrupts(LCD_IFLAG | VBL_IFLAG);
  enable_interrupts();

  sound_init();
  __critical {
    hUGE_init(&cognition);
    // add_VBL(hUGE_dosound);
  };

  // text
  text_load_font();

  // joypad
  int joypad_previous, joypad_current = 0;

  // player
  player_x = ((SCREEN_WIDTH_TILE / 2) * TILE_SIZE) << 4;
  player_y = ((SCREEN_HEIGHT_TILE / 2) * TILE_SIZE) << 4;
  player_draw_x = player_x >> 4;
  player_draw_y = player_y >> 4;

  set_sprite_data(0, mario_TILE_COUNT, mario_tiles);

  level_index = 0;
  load_area2();
  init_map();

  set_bkg_data(0, World1Tileset_TILE_COUNT, World1Tileset_tiles);

  score = 0;
  time = TIME_INITIAL_VALUE;
  lives = 3;
  coins = 0;

  vel_x = 0;
  vel_y = 0;

  display_jump_frame = FALSE;

  frame_counter = 0;
  bool mario_flip = FALSE;

  // text
  unsigned char windata[WINDOW_SIZE];
  memset(windata, 15, WINDOW_SIZE);
  set_win_tiles(0, 0, WINDOW_WIDTH_TILE, WINDOW_HEIGHT_TILE, windata);
  move_win(WINDOW_X, WINDOW_Y);

  char hud_line_one[SCREEN_WIDTH_TILE + 1] = "MARIOX01  WORLD TIME";

  hUGE_mute_channel(0, HT_CH_PLAY);
  hUGE_mute_channel(1, HT_CH_PLAY);
  hUGE_mute_channel(2, HT_CH_PLAY);
  hUGE_mute_channel(3, HT_CH_MUTE);

  text_print_string_win(0, 0, hud_line_one);
  text_print_string_win(1, 1, "0");
  text_print_string_win(8, 1, "X00");
  text_print_string_win(12, 1, "1-1");
  text_print_string_win(17, 1, "400");

  // display a coin in the HUD
  set_win_tile_xy(7, 1, 11);

  while (1) {
    // inputs
    joypad_previous = joypad_current;
    joypad_current = joypad();

#if defined(DEBUG)
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
#endif

    if (joypad_current & J_RIGHT) {
      vel_x = mario_speed;
      mario_flip = FALSE;
    }

    if (joypad_current & J_LEFT && player_x > 12) {
      vel_x = -mario_speed;
      mario_flip = TRUE;
    }

    // on release left pad
    if (!(joypad_current & J_LEFT) && (joypad_previous & J_LEFT)) {
      vel_x = 0;
    }

    // on release right pad
    if (!(joypad_current & J_RIGHT) && (joypad_previous & J_RIGHT)) {
      vel_x = 0;
    }

    if (joypad_current & J_A && !(joypad_previous & J_A)) {
      if (!is_jumping && touch_ground) {
        is_jumping = TRUE;
        display_jump_frame = TRUE;
        sound_play_jumping();
      }
    }

    // pause
    if (joypad_current & J_START && !(joypad_previous & J_START)) {
      sound_play_jumping();                 // TODO pause sound
      text_print_string_win(0, 0, "PAUSE"); // TODO bottom

      while (1) {
        joypad_current = joypad();
        // TODO if press start
        if (joypad_current & J_A && !(joypad_previous & J_A)) {
          break;
        }

        wait_vbl_done();
      }
    }

    if (joypad_current & J_B) {
      mario_speed = MARIO_SPEED_RUN;
    } else {
      mario_speed = MARIO_SPEED_WALK;
    }

    if (is_solid(player_draw_x, player_draw_y + 1)) {
      touch_ground = TRUE;
    }

    if (is_jumping) {
      vel_y = -JUMP_SPEED;
      current_jump++;
      if (current_jump > JUMP_MAX) {
        is_jumping = FALSE;
      }
    } else {
      vel_y = GRAVITY_SPEED;
    }

    // apply velocity to player coords
    if (vel_y != 0) {
      player_y_next = player_y + vel_y;
      player_draw_y_next = player_y_next >> 4;

      int16_t x_left = player_draw_x - MARIO_HALF_WIDTH;
      int16_t x_right = player_draw_x + MARIO_HALF_WIDTH - 1;

      // move down
      if (vel_y > 0) {
        if (is_solid(x_left, player_draw_y_next) ||
            is_solid(x_right, player_draw_y_next)) {
          uint8_t index_y = player_draw_y_next / TILE_SIZE;
          player_y = (index_y * TILE_SIZE) << 4;
          touch_ground = TRUE;
          current_jump = 0;
          is_jumping = FALSE;
          display_jump_frame = FALSE;
        } else {
          touch_ground = FALSE;
          player_y = player_y_next;
        }
      }

      // move up
      else if (vel_y < 0) {
        int y_top = player_draw_y_next - 6;
        if (is_solid(x_left, y_top) || is_solid(x_right, y_top)) {
          uint8_t index_y = player_draw_y_next / TILE_SIZE;
          player_y = (index_y * TILE_SIZE + TILE_SIZE) << 4;
          current_jump = 0;
          is_jumping = FALSE;
          sound_play_bump();
        } else {
          player_y = player_y_next;
        }
      }
      player_draw_y = player_y >> 4;
    }

    if (vel_x != 0) {
      player_x_next = player_x + vel_x;
      player_draw_x_next = player_x_next >> 4;

      int16_t y_bottom = player_draw_y - 1;
      int16_t y_top = player_draw_y - MARIO_HALF_WIDTH;

      // move right
      if (vel_x > 0 && player_draw_x / TILE_SIZE < map_width * TILE_SIZE) {
        int x_right = player_draw_x_next + MARIO_HALF_WIDTH;
        if (is_solid(x_right, y_top) || is_solid(x_right, y_bottom)) {
          uint8_t index_x = player_draw_x_next / TILE_SIZE;
          player_x = (index_x * TILE_SIZE + MARIO_HALF_WIDTH) << 4;
        } else {
          player_x = player_x_next;
        }
      }
      // move left
      else if (vel_x < 0) {
        int x_left = player_draw_x_next - MARIO_HALF_WIDTH;
        if (is_solid(x_left, y_top) || is_solid(x_left, y_bottom)) {
          uint8_t index_x = player_draw_x_next / TILE_SIZE;
          player_x = (index_x * TILE_SIZE + TILE_SIZE - MARIO_HALF_WIDTH) << 4;
        } else if (player_draw_x_next - camera_x > 1) {
          player_x = player_x_next;
        }
      }
      player_draw_x = player_x >> 4;
    }

    // set player frame
    if (display_jump_frame) {
      mario_current_frame = 4;
    } else if (vel_x != 0) {
      update_frame_counter();
    } else {
      mario_current_frame = 0;
    }

    // draw player
    int player_draw_x_camera_offset = player_draw_x - camera_x + TILE_SIZE;
    metasprite_t *mario_metasprite = mario_metasprites[mario_current_frame];
    if (mario_flip) {
      move_metasprite_vflip(mario_metasprite, 0, 0, player_draw_x_camera_offset,
                            player_draw_y);
    } else {
      move_metasprite(mario_metasprite, 0, 0, player_draw_x_camera_offset,
                      player_draw_y);
    }

    time--;
    if (time == 0) {
      time = TIME_INITIAL_VALUE;
      lives--;
    }

    // check coin
    if (is_coin(player_draw_x, player_draw_y)) {
      // remove coin (set to blank tile)
      set_bkg_tile_xy(player_draw_x / TILE_SIZE, player_draw_y / TILE_SIZE, 15);

      sound_play_bump(); // TODO play sound coin

      coins++;
      score += 100;

      if (coins == 100) {
        lives++;
        coins = 0;
      }

      hud_update_coins();
      hud_update_score();
    }

    // print DEBUG text
#if defined(DEBUG)
    char buffer[WINDOW_SIZE + 1];
    char fmt[] = "X:%d;XD:%d;\nCX:%d;VX:%d;";
    sprintf(buffer, fmt, (int16_t)player_x, (int16_t)player_draw_x,
            (int16_t)camera_x, vel_x);
    text_print_string_win(0, 0, buffer);
#endif

    // scroll
    if (vel_x > 0 && player_draw_x - camera_x > SCREENWIDTH / 2 &&
        camera_x < camera_max_x) {
      camera_x_mask += vel_x;
      camera_x  = camera_x_mask >> 4;
      redraw = TRUE;
    }

    if (redraw) {
      set_camera(map_height, map_width, map);
      redraw = FALSE;
    }

    hUGE_dosound();
    wait_vbl_done();
  }
}
