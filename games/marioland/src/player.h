#ifndef PLAYER_H
#define PLAYER_H

#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "sound.h"
#include "graphics/mario.h"
#include "level.h"

// player coords and movements
extern uint16_t player_x_subpixel;
extern uint16_t player_y_subpixel;
extern uint16_t player_x_subpixel_next;
extern uint16_t player_y_subpixel_next;
extern uint8_t player_draw_x;
extern uint8_t player_draw_y;
extern uint8_t player_draw_x_next;
extern uint8_t player_draw_y_next;
extern int8_t vel_x;
extern int8_t vel_y;
extern bool is_jumping;
extern bool display_jump_frame;
extern bool display_slide_frame;
extern bool touch_ground;
extern uint8_t current_jump;
extern int8_t player_max_speed;
extern uint8_t player_frame;
extern uint8_t frame_counter;
extern bool mario_flip;
extern uint8_t current_gravity;

extern uint8_t x_right_draw;
extern uint8_t x_left_draw;
extern uint8_t y_top_draw;
extern uint8_t y_bottom_draw;

extern uint8_t next_pos;
extern uint8_t tile_next_1;
extern uint8_t tile_next_2;

void update_frame_counter() BANKED;
void player_move() BANKED;
void player_draw();

#endif