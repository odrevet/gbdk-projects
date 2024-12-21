#ifndef GAME_H
#define GAME_H

#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hUGEDriver.h"
#include "global.h"
#include "level.h"
#include "text.h"
#include "player.h"

void init();
void die();
void on_get_coin(uint8_t x, uint8_t y);
void hud_update_coins();
void hud_update_score();
void hud_update_time();
void hud_update_lives();

#endif

