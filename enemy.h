#ifndef ENEMY_H
#define ENEMY_H

#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#include "structs.h"
#include "levels.h"

void enemy_move(enemy_t *enemy, hero_t *hero, unsigned char *map);

#endif