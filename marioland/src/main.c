#include <gb/gb.h>
#include <gb/metasprites.h>
#include <gbdk/incbin.h>
#include <gbdk/platform.h>
#include <gbdk/rledecompress.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/enemies.h"
#include "graphics/mario.h"

#include "enemy.h"
#include "global.h"
#include "hUGEDriver.h"
#include "level.h"
#include "sound.h"
#include "text.h"

#include "../res/level_1_1.h"
INCBIN(level_map_bin_rle, "res/level_1_1_map.bin.rle")
INCBIN_EXTERN(level_map_bin_rle)

INCBIN(level_tiles_bin, "res/level_1_1_tiles.bin")
INCBIN_EXTERN(level_tiles_bin)

#define DEVICE_SCREEN_PX_WIDTH_HALF DEVICE_SCREEN_PX_WIDTH / 2
#define LEVEL_HEIGHT 16

uint8_t coldata[LEVEL_HEIGHT]; // buffer for one columns
uint8_t mapdata[LEVEL_HEIGHT * DEVICE_SCREEN_WIDTH + 1]; // buffer for map chunk
uint8_t datapos = 0;
int16_t half_screen_player_overflow = 0;
int16_t prevhalf_screen_player_overflow = 0;

const uint8_t window_location = WINDOW_Y + WINDOW_HEIGHT_TILE * TILE_SIZE;

uint8_t coins;
uint16_t score;
bool redraw;
uint8_t joy;
uint16_t time;
uint8_t lives;
uint8_t level_index;

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
bool display_slide_frame;
bool touch_ground = FALSE;
uint8_t current_jump = 0;
int8_t player_max_speed = PLAYER_MAX_SPEED_WALK;
uint8_t player_current_frame = 0;
uint8_t frame_counter = 0;
bool mario_flip;

// music
// extern const hUGESong_t fish_n_chips;
extern const hUGESong_t cognition;

inline bool is_solid_mapdata(int x, int y) {
  const unsigned char tile =
      mapdata[DEVICE_SCREEN_WIDTH * (y / TILE_SIZE) + (x / TILE_SIZE)];
  return tile == 0x22;
}

void update_frame_counter() {
  frame_counter++;
  if (frame_counter == LOOP_PER_ANIMATION_FRAME) {
    frame_counter = 0;
    player_current_frame = (player_current_frame % 3) + 1;
  }
}

void init_map() {
  SCX_REG = 0;
  // map_width = level_1_1_WIDTH;
  // map_height = level_1_1_HEIGHT;
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

inline void on_get_coin(int x_right, int y_bottom) {
  set_bkg_tile_xy(x_right / TILE_SIZE, y_bottom / TILE_SIZE, 15);

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

void player_draw() {
  metasprite_t *mario_metasprite = mario_metasprites[player_current_frame];
  if (mario_flip) {
    move_metasprite_vflip(
        mario_metasprite, 0, 0, player_draw_x + DEVICE_SPRITE_PX_OFFSET_X,
        player_draw_y + DEVICE_SPRITE_PX_OFFSET_Y - TILE_SIZE);
  } else {
    move_metasprite(mario_metasprite, 0, 0,
                    player_draw_x + DEVICE_SPRITE_PX_OFFSET_X,
                    player_draw_y + DEVICE_SPRITE_PX_OFFSET_Y - TILE_SIZE);
  }
}

void interruptLCD() {
  while (STAT_REG & 3)
    ;
  HIDE_WIN;
}

void interruptVBL() { SHOW_WIN; }

void main(void) {
  DISPLAY_ON;
  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  SPRITES_8x16;

  STAT_REG = 0x40;
  LYC_REG = 0x0F;

  move_bkg(0, -16);

  disable_interrupts();
  add_LCD(interruptLCD);
  add_VBL(interruptVBL);
  set_interrupts(VBL_IFLAG | LCD_IFLAG);
  enable_interrupts();

  sound_init();
  __critical {
    hUGE_init(&cognition);
    add_VBL(hUGE_dosound);
  };

  // text
  text_load_font();

  // joypad
  int joypad_previous, joypad_current = 0;

  // player
  player_x = ((DEVICE_SCREEN_WIDTH / 2) * TILE_SIZE) << 4;
  player_y = ((DEVICE_SCREEN_HEIGHT / 2) * TILE_SIZE) << 4;
  player_draw_x = player_x >> 4;
  player_draw_y = player_y >> 4;

  set_sprite_data(SPRITE_START_MARIO, mario_TILE_COUNT, mario_tiles);
  set_sprite_data(SPRITE_START_ENEMIES, enemies_TILE_COUNT, enemies_tiles);

  level_index = 0;

  init_map();

  score = 0;
  time = TIME_INITIAL_VALUE;
  lives = 3;
  coins = 0;

  vel_x = 0;
  vel_y = 0;

  display_jump_frame = FALSE;
  display_slide_frame = FALSE;

  frame_counter = 0;
  mario_flip = FALSE;

  // text
  unsigned char windata[WINDOW_SIZE];
  memset(windata, 15, WINDOW_SIZE);
  set_win_tiles(0, 0, WINDOW_WIDTH_TILE, WINDOW_HEIGHT_TILE, windata);
  move_win(WINDOW_X, WINDOW_Y);

  char hud_line_one[DEVICE_SCREEN_WIDTH + 1] = "MARIOX01  WORLD TIME";

  hUGE_mute_channel(0, HT_CH_PLAY);
  hUGE_mute_channel(1, HT_CH_PLAY);
  hUGE_mute_channel(2, HT_CH_PLAY);
  hUGE_mute_channel(3, HT_CH_PLAY);

  text_print_string_win(0, 0, hud_line_one);
  text_print_string_win(0, 1, "   0   X00  1-1  400  ");

  // display a coin in the HUD
  // set_win_tile_xy(7, 1, 11);

  // spawn enemies
  // enemy_new(50, 136, ENEMY_TYPE_GOOMBA);
  // enemy_new(70, 136, ENEMY_TYPE_KOOPA);

  // map
  uint16_t mapdata_index = 0;

  rle_init(level_map_bin_rle);
  set_bkg_data(0, INCBIN_SIZE(level_tiles_bin) >> 4, level_tiles_bin);

  for (uint8_t index_col = 0; (index_col != DEVICE_SCREEN_WIDTH + 1);
       index_col++) {
    // decompress a column of tile data
    rle_decompress(coldata, LEVEL_HEIGHT);

    // uint8_t coldatax[DEVICE_SCREEN_HEIGHT] = {0, 0, 0, 0, 0, 0, 0x22};
    // memcpy(coldata, coldatax, DEVICE_SCREEN_HEIGHT);

    // copy to VRAM
    set_bkg_tiles(index_col & (DEVICE_SCREEN_BUFFER_WIDTH - 1), 0, 1,
                  DEVICE_SCREEN_HEIGHT, coldata);

    // copy coldata to mapdata column by column
    for (uint8_t i = 0; i < LEVEL_HEIGHT; i++) {
      mapdata[i * DEVICE_SCREEN_WIDTH + index_col] = coldata[i];
    }
  }

  while (1) {
    // inputs
    joypad_previous = joypad_current;
    joypad_current = joypad();

    if (joypad_current & J_RIGHT) {
      if (vel_x < player_max_speed) {
        vel_x += 1;
        mario_flip = FALSE;
        if (vel_x < 0) {
          display_slide_frame = TRUE;
        } else {
          display_slide_frame = FALSE;
        }
      } else if (vel_x > player_max_speed) {
        vel_x -= 1;
      }
    } else if (vel_x > 0) {
      vel_x -= 1;
    }

    if ((joypad_current & J_LEFT)) {
      if (player_x > 12 && abs(vel_x) < player_max_speed) {
        vel_x -= 1;
        mario_flip = TRUE;
        if (vel_x > 0) {
          display_slide_frame = TRUE;
        } else {
          display_slide_frame = FALSE;
        }
      } else if (abs(vel_x) > player_max_speed) {
        vel_x += 1;
      }
    } else if (vel_x < 0) {
      vel_x += 1;
    }

    if (joypad_current & J_A && !(joypad_previous & J_A) && !is_jumping &&
        touch_ground) {
      is_jumping = TRUE;
      display_jump_frame = TRUE;
      // sound_play_jumping();
    }

    // pause
    /*if (joypad_current & J_START && !(joypad_previous & J_START)) {
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
    }*/

    if (joypad_current & J_B) {
      player_max_speed = PLAYER_MAX_SPEED_RUN;
    } else {
      player_max_speed = PLAYER_MAX_SPEED_WALK;
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

    int16_t x_right_draw = player_draw_x + MARIO_HALF_WIDTH - 1;
    int16_t x_left_draw = player_draw_x - MARIO_HALF_WIDTH;

    // apply velocity to player coords
    if (vel_y != 0) {
      player_y_next = player_y + vel_y;
      player_draw_y_next = player_y_next >> 4;

      // move down
      if (vel_y > 0) {
        int16_t y_bottom_next = player_draw_y_next;
        if (is_solid(x_left_draw, y_bottom_next) ||
            is_solid(x_right_draw, y_bottom_next)) {
          uint8_t index_y = y_bottom_next / TILE_SIZE;
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
        int16_t y_top_next = player_draw_y_next - 6;
        if (is_solid(x_left_draw, y_top_next) ||
            is_solid(x_right_draw, y_top_next)) {
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

    int16_t y_top_draw = player_draw_y - MARIO_HALF_WIDTH;
    int16_t y_bottom_draw = player_draw_y - 1;

    if (vel_x != 0) {
      player_x_next = player_x + vel_x;
      player_draw_x_next = player_x_next >> 4;

      // move right
      if (vel_x > 0) {
        int16_t x_right_next = player_draw_x_next + MARIO_HALF_WIDTH;
        if (is_solid(x_right_next, y_top_draw) ||
            is_solid(x_right_next, y_bottom_draw)) {
          uint8_t index_x = player_draw_x_next / TILE_SIZE;
          player_x = (index_x * TILE_SIZE + MARIO_HALF_WIDTH) << 4;
        } else {
          player_x = player_x_next;
        }
      }
      // move left
      else if (vel_x < 0) {
        int16_t x_left_next = player_draw_x_next - MARIO_HALF_WIDTH;
        if (is_solid(x_left_next, y_top_draw) ||
            is_solid(x_left_next, y_bottom_draw)) {
          uint8_t index_x = player_draw_x_next / TILE_SIZE;
          player_x = (index_x * TILE_SIZE + TILE_SIZE - MARIO_HALF_WIDTH) << 4;
        } else if (player_draw_x_next > 1) {
          player_x = player_x_next;
        }
      }

      player_draw_x = player_x >> 4;
      half_screen_player_overflow = player_draw_x - DEVICE_SCREEN_PX_WIDTH_HALF;
    }

    // set player frame
    if (display_jump_frame) {
      player_current_frame = 4;
    } else if (vel_x != 0) {
      if (display_slide_frame) {
        player_current_frame = 5;
      } else {
        update_frame_counter();
      }
    } else {
      player_current_frame = 0;
    }

    player_draw();
    // enemy_update();
    // enemy_draw(SPRITE_START_ENEMIES);

    // print DEBUG text
#if defined(DEBUG)
    char buffer[WINDOW_SIZE + 1];
    char fmt[] = "X:%d;X DRAW:%d;\nXV:%d;XOF:%d;HL:%d;";
    sprintf(buffer, fmt, (int16_t)player_x, (int16_t)player_draw_x, vel_x,
            half_screen_player_overflow, DEVICE_SCREEN_HEIGHT);
    text_print_string_win(0, 0, buffer);
#endif

    time--;
    if (time == 0) {
      time = TIME_INITIAL_VALUE;
      lives--;
    }

    // check coins
    /*
    if (is_coin(x_right_draw, y_bottom_draw - TILE_SIZE)) {
      on_get_coin(x_right_draw, y_bottom_draw - TILE_SIZE);
    }

    if (is_coin(x_right_draw, y_top_draw)) {
      on_get_coin(x_right_draw, y_top_draw);
    }

    if (is_coin(x_left_draw, y_bottom_draw - TILE_SIZE)) {
      on_get_coin(x_left_draw, y_bottom_draw - TILE_SIZE);
    }

    if (is_coin(x_left_draw, y_top_draw)) {
      on_get_coin(x_left_draw, y_top_draw);
    }*/

    wait_vbl_done();

    // scroll
    if (half_screen_player_overflow > 0) {
      player_x = DEVICE_SCREEN_PX_WIDTH_HALF << 4;
      SCX_REG += half_screen_player_overflow;
      prevhalf_screen_player_overflow += half_screen_player_overflow;
      half_screen_player_overflow = 0;

      if (prevhalf_screen_player_overflow >= TILE_SIZE) {
        prevhalf_screen_player_overflow -= TILE_SIZE;
        datapos = (SCX_REG >> 3);
        uint8_t map_x_column =
            (datapos + DEVICE_SCREEN_WIDTH) & (DEVICE_SCREEN_BUFFER_WIDTH - 1);

        if (!rle_decompress(coldata, LEVEL_HEIGHT)) {
          // no more data
        }

        set_bkg_tiles(map_x_column, 0, 1, LEVEL_HEIGHT, coldata);
      }
    }
  }
}
