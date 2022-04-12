#ifndef LEVELS_H
#define LEVELS_H

#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#include "structs.h"
#include "hero.h"

#include "maps/map_001.h"
#include "maps/map_002.h"
#include "maps/map_003.h"

void init_level_001(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map);
void init_level_002(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map);
void init_level_003(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map);

inline unsigned char get_tile(unsigned short x, unsigned short y, unsigned char *map);

inline void set_tile(unsigned short index_x, unsigned short index_y, unsigned char value, unsigned char *map);

bool has_collision_xy(int a_x, int a_y, int b_x, int b_y);

inline bool can_move(int x, int y, unsigned char *map);

unsigned short get_possible_moves(int x, int y, unsigned char *map);

unsigned short get_possible_moves_bat(int x, int y, unsigned char *map);

#endif