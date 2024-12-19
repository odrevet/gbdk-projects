#include "global.h"

const uint8_t window_location = WINDOW_Y + WINDOW_HEIGHT_TILE * TILE_SIZE;

uint8_t coins;
uint16_t score;
uint8_t joy;
uint16_t time;
uint8_t lives;
uint8_t joypad_previous, joypad_current;
uint8_t nb_col = COLUMN_CHUNK_SIZE;
uint8_t scroll;

