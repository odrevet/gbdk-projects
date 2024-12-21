#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "hUGEDriver.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

#include "sound.h"
#include "text.h"
#include "level.h"


// Graphics
#include "graphics/enemies.h"
#include "graphics/mario.h"
#include "graphics/text.h"
#include "graphics/common.h"



// music
extern const hUGESong_t overworld;


void interruptLCD() {
  while (STAT_REG & 3)
    ;
  HIDE_WIN;
}

void interruptVBL() { SHOW_WIN; }

void pause() {
  hUGE_mute_channel(0, HT_CH_MUTE);
  hUGE_mute_channel(1, HT_CH_MUTE);
  hUGE_mute_channel(2, HT_CH_MUTE);
  hUGE_mute_channel(3, HT_CH_MUTE);

  sound_play_bump();
  text_print_string_win(DEVICE_SCREEN_WIDTH - 5, 1, "PAUSE");

#if defined(DEBUG)
  // debug a column of background
  char buffer[WINDOW_SIZE + 1];
  char fmt[] = "%d.%d.%d.%d.%d.%d.%d..";
  uint8_t col = player_draw_x / TILE_SIZE;
  
  for (uint8_t row = 0; row < 7; row++) {
    uint16_t index = (row * MAP_BUFFER_WIDTH) + col;
    sprintf(buffer, fmt, 
            map_buffer[index], 
            map_buffer[index + MAP_BUFFER_WIDTH], 
            map_buffer[index + 2 * MAP_BUFFER_WIDTH], 
            map_buffer[index + 3 * MAP_BUFFER_WIDTH], 
            map_buffer[index + 4 * MAP_BUFFER_WIDTH], 
            map_buffer[index + 5 * MAP_BUFFER_WIDTH], 
            map_buffer[index + 6 * MAP_BUFFER_WIDTH]);
    text_print_string_win(0, row, buffer);
  }
#endif


  vsync();

  while (1) {
    joypad_previous = joypad_current;
    joypad_current = joypad();
    if (joypad_current & J_START && !(joypad_previous & J_START)) {
      break;
    }
  }

  hUGE_mute_channel(0, HT_CH_PLAY);
  hUGE_mute_channel(1, HT_CH_PLAY);
  hUGE_mute_channel(2, HT_CH_PLAY);
  hUGE_mute_channel(3, HT_CH_PLAY);

  text_print_string_win(DEVICE_SCREEN_WIDTH - 5, 1, "     ");
  hud_update_time();
}





void main(void) {
  STAT_REG = 0x40;
  LYC_REG = 0x0F;

  move_bkg(0, -MARGIN_TOP_PX);

  disable_interrupts();
  add_LCD(interruptLCD);
  add_VBL(interruptVBL);
  set_interrupts(VBL_IFLAG | LCD_IFLAG);
  enable_interrupts();

  sound_init();
  __critical {
    hUGE_init(&overworld);
    add_VBL(hUGE_dosound);
  };

  // joypad
  joypad_previous, joypad_current = 0;

  // player
  player_x_subpixel = 43 << 4;
  player_y_subpixel = (16 * TILE_SIZE) << 4;
  player_draw_x = player_x_subpixel >> 4;
  player_draw_y = player_y_subpixel >> 4;


  init();

  score = 0;
  lives = INITIAL_LIVES;
  coins = 0;

  // HUD
  // text
  unsigned char windata[WINDOW_SIZE];
  memset(windata, 15, WINDOW_SIZE);
  set_win_tiles(0, 0, WINDOW_WIDTH_TILE, WINDOW_HEIGHT_TILE, windata);
  move_win(WINDOW_X, WINDOW_Y);
  text_print_string_win(0, 0, "MARIOx00  WORLD TIME");
  text_print_string_win(0, 1, "     0  x00 1-1  000");

  // display a coin in the HUD
  set_win_tile_xy(7, 1, TILE_COIN);
  hud_update_time();
  hud_update_lives();

  // Set music channels channel
  hUGE_mute_channel(0, HT_CH_PLAY);
  hUGE_mute_channel(1, HT_CH_PLAY);
  hUGE_mute_channel(2, HT_CH_PLAY);
  hUGE_mute_channel(3, HT_CH_PLAY);

  // spawn enemies
  // enemy_new(50, 136, ENEMY_TYPE_GOOMBA);
  // enemy_new(70, 136, ENEMY_TYPE_KOOPA);

  // Initialize graphics
  set_sprite_data(SPRITE_START_MARIO, mario_TILE_COUNT, mario_tiles);
  set_sprite_data(SPRITE_START_ENEMIES, enemies_TILE_COUNT, enemies_tiles);
  set_bkg_data(text_TILE_ORIGIN, text_TILE_COUNT, text_tiles);
  set_bkg_data(common_TILE_ORIGIN, common_TILE_COUNT, common_tiles);

  current_level = 0;
  level_set_current();
  level_load_current();


  DISPLAY_ON;
  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  SPRITES_8x16;

  while (1) {
    vsync();

    // inputs
    joypad_previous = joypad_current;
    joypad_current = joypad();

    player_move();


    // pause
    if (joypad_current & J_START && !(joypad_previous & J_START)) {
      pause();
    }

    x_right_draw = player_draw_x + MARIO_HALF_WIDTH - 1;
    x_left_draw = player_draw_x - MARIO_HALF_WIDTH;
    y_top_draw = player_draw_y - MARIO_HALF_WIDTH;
    y_bottom_draw = player_draw_y - 1;

   // apply velocity to player coords
    if (vel_x != 0) {
      player_x_subpixel_next = player_x_subpixel + vel_x;
      player_draw_x_next = player_x_subpixel_next >> 4;

      // move right
      if (vel_x > 0) {
        next_pos = player_draw_x_next + MARIO_HALF_WIDTH;
        tile_next_1 = level_get_tile(next_pos, y_top_draw);    // tile_right_top
        tile_next_2 = level_get_tile(next_pos, y_bottom_draw); // tile_right_bottom

        if (level_is_tile_solid(tile_next_1) || level_is_tile_solid(tile_next_2)) {
          vel_x = 0;
          uint8_t diff = camera_x % TILE_SIZE;

          // (((player_draw_x_next / TILE_SIZE) + 1) * TILE_SIZE)
          player_draw_x =
              (((player_draw_x_next & ~(TILE_SIZE - 1)) + TILE_SIZE) - diff -
               MARIO_HALF_WIDTH) +
              (diff > 4 ? 8 : 0);
          player_x_subpixel = player_draw_x << 4;

        } else {
          if (is_coin(tile_next_1)) {
            on_get_coin(next_pos, y_top_draw);
          }

          if (is_coin(tile_next_2)) {
            on_get_coin(next_pos, y_bottom_draw);
          }
          player_x_subpixel = player_x_subpixel_next;
          player_draw_x = player_x_subpixel >> 4;
        }

        // scroll
        if (!level_end_reached && player_draw_x >= DEVICE_SCREEN_PX_WIDTH_HALF) {
          scroll = player_x_subpixel - (DEVICE_SCREEN_PX_WIDTH_HALF << 4);

          camera_x_subpixel += scroll;
          camera_x = camera_x_subpixel >> 4;
          SCX_REG = camera_x;

          player_x_subpixel = DEVICE_SCREEN_PX_WIDTH_HALF << 4;
          player_draw_x = DEVICE_SCREEN_PX_WIDTH_HALF;

          if (camera_x >> 3 >= next_col_chunk_load) {
            nb_col = level_load_column(next_col_chunk_load + DEVICE_SCREEN_WIDTH, COLUMN_CHUNK_SIZE);

            if (nb_col == 0) {
              level_end_reached = true;
              nb_col = COLUMN_CHUNK_SIZE;
            } else {
              next_col_chunk_load += nb_col;
            }
          }
        }
      }
      else if (vel_x < 0) {
        next_pos = player_draw_x_next - MARIO_HALF_WIDTH;
        tile_next_1 = level_get_tile(next_pos, y_top_draw);    // tile_left_top
        tile_next_2 = level_get_tile(next_pos, y_bottom_draw); // tile_left_bottom

        if (player_draw_x_next < MARIO_HALF_WIDTH) {
          player_draw_x = MARIO_HALF_WIDTH;
          player_x_subpixel = player_draw_x << 4;
        } else {
          if (level_is_tile_solid(tile_next_1) || level_is_tile_solid(tile_next_2)) {
            vel_x = 0;
            uint8_t diff = camera_x % TILE_SIZE;

            // (((player_draw_x_next / TILE_SIZE) + 1) * TILE_SIZE)
            player_draw_x =
                ((((player_draw_x_next & ~(TILE_SIZE - 1)) + TILE_SIZE)) -
                 diff - MARIO_HALF_WIDTH) +
                (diff < 4 ? 0 : 8);
            player_x_subpixel = player_draw_x << 4;
          } else {
            if (is_coin(tile_next_1)) {
              on_get_coin(next_pos, y_top_draw);
            }

            if (is_coin(tile_next_2)) {
              on_get_coin(next_pos, y_bottom_draw);
            }

            player_x_subpixel = player_x_subpixel_next;
            player_draw_x = player_x_subpixel >> 4;
          }
        }
      }
    }

    if (vel_y != 0) {
      player_y_subpixel_next = player_y_subpixel + vel_y;
      player_draw_y_next = player_y_subpixel_next >> 4;

      // move down
      if (vel_y > 0) {
        next_pos = player_draw_y_next;
        uint8_t tile_left_bottom = level_get_tile(x_left_draw, next_pos);
        uint8_t tile_right_bottom = level_get_tile(x_right_draw, next_pos);

        if (level_is_tile_solid(tile_left_bottom) || level_is_tile_solid(tile_right_bottom)) {
           player_y_subpixel = ((next_pos / TILE_SIZE) * TILE_SIZE) << 4;

          touch_ground = TRUE;
          current_jump = 0;
          is_jumping = FALSE;
          display_jump_frame = FALSE;
        } else {
          if (is_coin(tile_left_bottom)) {
            on_get_coin(x_left_draw, next_pos);
          }

          if (is_coin(tile_right_bottom)) {
            on_get_coin(x_right_draw, next_pos);
          }

//          touch_ground = FALSE;
//          player_y_subpixel = player_y_subpixel_next;
        }
      }
      else if (vel_y < 0) {
//        next_pos = player_draw_y_next - 6;
//        uint8_t tile_left_top = level_get_tile(x_left_draw, next_pos);
//        uint8_t tile_right_top = level_get_tile(x_right_draw, next_pos);
//
//        if (level_is_tile_solid(tile_left_top) || level_is_tile_solid(tile_right_top)) {
//           player_y_subpixel =
//               ((player_draw_y_next / TILE_SIZE) * TILE_SIZE + TILE_SIZE) <<
//               4;
//
//          current_jump = 0;
//          is_jumping = FALSE;
//          sound_play_bump();
//        } else {
//          if (is_coin(tile_left_top)) {
//            on_get_coin(x_left_draw, next_pos);
//          }
//
//          if (is_coin(tile_right_top)) {
//            on_get_coin(x_right_draw, next_pos);
//          }
//          player_y_subpixel = player_y_subpixel_next;
//        }
      }
      player_draw_y = player_y_subpixel >> 4;
    }



    // enemy_update();
    // enemy_draw(SPRITE_START_ENEMIES);


    if (joypad_current & J_SELECT && !(joypad_previous & J_SELECT)) {
      init();
      (++current_level) % NB_LEVELS;
      level_set_current();
    }


    // print DEBUG text
#if defined(DEBUG)
    char buffer[WINDOW_SIZE + 1];
    sprintf(buffer, 
            "%d.%d.%d.%d.%d.\n.%d.%d.%d.%d.", 
            (uint16_t)player_x_subpixel,
            (uint16_t)player_y_subpixel, 
            player_draw_x, 
            vel_x, 
            vel_y,
            camera_x, 
            scroll, 
            current_map_width, 
            level_end_reached);
    text_print_string_win(0, 0, buffer);
#else
    time--;
    hud_update_time();
    if (time == 0) {
      time = TIME_INITIAL_VALUE;
      lives--;
      hud_update_lives();
    }
#endif

    // fall under screen
    if (player_draw_y > DEVICE_SCREEN_PX_HEIGHT) {
      die();
    }

    // reach end of level
    if(level_end_reached && player_draw_x >= (DEVICE_SCREEN_WIDTH - 2) * TILE_SIZE){
      init();
      (++current_level) % NB_LEVELS;
      level_set_current();
    }

    player_draw();
  }
}
