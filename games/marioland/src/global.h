#ifndef GLOBAL_H
#define GLOBAL_H

#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SPRITE_START_MARIO 0
#define SPRITE_START_ENEMIES SPRITE_START_MARIO + mario_TILE_COUNT

#define TILE_SIZE 8
#define DEVICE_SCREEN_HEIGHT 18
#define DEVICE_SCREEN_WIDTH 20
#define WINDOW_HEIGHT_TILE 2
#define WINDOW_WIDTH_TILE DEVICE_SCREEN_WIDTH
#define WINDOW_SIZE WINDOW_WIDTH_TILE *WINDOW_HEIGHT_TILE
#define WINDOW_X MINWNDPOSX
#define WINDOW_Y MINWNDPOSY

#define TIME_INITIAL_VALUE 400 * 40
#define GRAVITY 96
#define GRAVITY_JUMP 5
#define JUMP_SPEED 80
#define PLAYER_MAX_SPEED_WALK 20
#define PLAYER_MAX_SPEED_RUN 28
#define TERMINAL_VELOCITY 96
#define LOOP_PER_ANIMATION_FRAME 5
#define MARIO_HALF_WIDTH 4

#define DEVICE_SCREEN_PX_WIDTH_HALF DEVICE_SCREEN_PX_WIDTH / 2
#define MARGIN_TOP 2
#define MARGIN_TOP_PX 2 * TILE_SIZE
#define DEVICE_SPRITE_OFFSET_Y 2

#define NB_LEVELS 1
#define LEVEL_HEIGHT 16
#define COLUMN_CHUNK_SIZE 3 // how many map columns to load at a time
#define INITIAL_LIVES 3

extern const uint8_t window_location;
extern uint8_t coins;
extern uint16_t score;
extern uint8_t joy;
extern uint16_t time;
extern uint8_t lives;
extern uint8_t joypad_previous, joypad_current;
extern uint8_t nb_col;
extern uint8_t scroll;

#endif
