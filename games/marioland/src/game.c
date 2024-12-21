#pragma bank 255

#include "game.h"

void init() {
  time = TIME_INITIAL_VALUE;

  camera_x = 0;
  camera_x_subpixel = 0;
  SCX_REG = 0;

  player_x_subpixel = 43 << 4;
  player_y_subpixel = (16 * TILE_SIZE) << 4;
  player_draw_x = player_x_subpixel >> 4;
  player_draw_y = player_y_subpixel >> 4;

  vel_x = 0;
  vel_y = 0;

  display_jump_frame = FALSE;
  display_slide_frame = FALSE;

  frame_counter = 0;
  mario_flip = FALSE;
}

void die() {
  hUGE_mute_channel(0, HT_CH_PLAY);
  hUGE_mute_channel(1, HT_CH_PLAY);
  hUGE_mute_channel(2, HT_CH_PLAY);
  hUGE_mute_channel(3, HT_CH_PLAY);

  init();

  lives--;
  if(lives == 0){
    lives = INITIAL_LIVES;
    current_level = 0;
    level_set_1_1();
  }

  hud_update_lives();
  level_load_current();
}

void on_get_coin(uint8_t x, uint8_t y) {
  uint16_t index = ((y / TILE_SIZE - DEVICE_SPRITE_OFFSET_Y) * MAP_BUFFER_WIDTH) + 
                   (((x + camera_x) / TILE_SIZE) % MAP_BUFFER_WIDTH);
  map_buffer[index] = TILE_EMPTY;

  set_bkg_tile_xy(((x + camera_x) / TILE_SIZE) % DEVICE_SCREEN_BUFFER_WIDTH,
                  y / TILE_SIZE - DEVICE_SPRITE_OFFSET_Y, TILE_EMPTY);

  sound_play_coin();

  coins++;
  score += 100;

  if (coins == 100) {
    lives++;
    coins = 0;
  }

  hud_update_coins();
  hud_update_score();
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
  char score_str[5];
  itoa(score, score_str, 10);
  text_print_string_win(3, 1, score_str);
}

void hud_update_time() {
  char time_str[4];
  itoa(time / 40, time_str, 10);
  text_print_string_win(DEVICE_SCREEN_WIDTH - 3, 1, "000");
  text_print_string_win(DEVICE_SCREEN_WIDTH - strlen(time_str), 1, time_str);
}

void hud_update_lives() {
  char lives_str[4];
  itoa(lives, lives_str, 10);
  // text_print_string_win(8, 0, "00");
  text_print_string_win(7, 0, lives_str);
}
