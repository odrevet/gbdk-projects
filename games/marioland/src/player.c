#pragma bank 255

#include "player.h"

uint16_t player_x_subpixel;
uint16_t player_y_subpixel;
uint16_t player_x_subpixel_next;
uint16_t player_y_subpixel_next;
uint8_t player_draw_x;
uint8_t player_draw_y;
uint8_t player_draw_x_next;
uint8_t player_draw_y_next;
int8_t vel_x;
int8_t vel_y;
bool is_jumping = FALSE;
bool display_jump_frame;
bool display_slide_frame;
bool touch_ground = FALSE;
uint8_t current_jump = 0;
int8_t player_max_speed = PLAYER_MAX_SPEED_WALK;
uint8_t player_frame = 0;
uint8_t frame_counter = 0;
bool mario_flip;
uint8_t current_gravity = GRAVITY;

uint8_t x_right_draw;
uint8_t x_left_draw;
uint8_t y_top_draw;
uint8_t y_bottom_draw;

uint8_t next_pos;
uint8_t tile_next_1;
uint8_t tile_next_2;

void update_frame_counter() {
  frame_counter++;
  if (frame_counter == LOOP_PER_ANIMATION_FRAME) {
    frame_counter = 0;
    player_frame = (player_frame % 3) + 1;
  }
}

void player_move() BANKED {
    if (joypad_current & J_B) {
      player_max_speed = PLAYER_MAX_SPEED_RUN;
    } else {
      player_max_speed = PLAYER_MAX_SPEED_WALK;
    }

    if (is_jumping) {
      vel_y += GRAVITY_JUMP;
      if (vel_y > TERMINAL_VELOCITY) {
        vel_y = TERMINAL_VELOCITY;
      }
    } else {
      vel_y = GRAVITY;
    }

    if (joypad_current & J_RIGHT) {
      if (vel_x < player_max_speed) {
        vel_x += 1;
        if(display_jump_frame == FALSE){
          mario_flip = FALSE;
          }
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
      if (abs(vel_x) < player_max_speed) {
        vel_x -= 1;
        if(display_jump_frame == FALSE){
          mario_flip = TRUE;
        }
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


    if (joypad_current & J_A && !(joypad_previous & J_A) && !is_jumping && touch_ground) {
      is_jumping = TRUE;
      display_jump_frame = TRUE;
      vel_y = -JUMP_SPEED;
      sound_play_jump();
    }
}

void player_draw() {
  // set player frame
  if (display_jump_frame) {
    player_frame = 4;
  } else if (vel_x != 0) {
    if (display_slide_frame) {
      player_frame = 5;
    } else {
      update_frame_counter();
    }
  } else {
    player_frame = 0;
  }

  metasprite_t *const mario_metasprite = mario_metasprites[player_frame];
  if (mario_flip) {
    move_metasprite_flipx(mario_metasprite, 0, 0, 0, player_draw_x + TILE_SIZE,
                          player_draw_y + DEVICE_SPRITE_PX_OFFSET_Y -
                              TILE_SIZE);
  } else {
    move_metasprite_ex(mario_metasprite, 0, 0, 0, player_draw_x + TILE_SIZE,
                       player_draw_y + DEVICE_SPRITE_PX_OFFSET_Y - TILE_SIZE);
  }
}
