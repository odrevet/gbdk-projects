#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdint.h>
#include <stdbool.h>

#include "graphics/linda.h"
#include "graphics/city.h"
#include "maps/map_001.h"

#define HERO_SPRITE_INDEX 0
#define SPR_NUM_START 0

uint16_t pos_x, pos_y;
int16_t speed_x, speed_y;
int16_t idx;
uint8_t animation_delay;

enum ANIMATION_INDEX
{
    ANIMATION_STILL,
    ANIMATION_WALK_0,
    ANIMATION_WALK_1,
    ANIMATION_JUMP,
    ANIMATION_CROUCH,
    ANIMATION_CLIMB
};

uint8_t walk_animation[] = {ANIMATION_STILL, ANIMATION_WALK_0, ANIMATION_STILL, ANIMATION_WALK_1};

const uint8_t ANIMATION_SPEED = 8;
const uint8_t SPEED = 8;
uint8_t is_crouching = false;
uint8_t is_facing_left = false;

int game(void)
{

    pos_x = pos_y = 64 << 4;
    speed_x = speed_y = 0;
    idx = 0;
    animation_delay = 0;

    set_bkg_data(0, 16, city_data);
    set_bkg_tiles(0, 0, map_001Width, map_001Height, map_001);

    set_sprite_data(HERO_SPRITE_INDEX, sizeof(linda_data) >> 4, linda_data);

    while (1)
    {
        switch (joypad())
        {
        case J_LEFT:
            speed_x -= SPEED;
            is_facing_left = true;
            break;
        case J_RIGHT:
            speed_x += SPEED;
            is_facing_left = false;
            break;
        case J_DOWN:
            speed_y += SPEED;
            break;
        case J_UP:
            speed_y -= SPEED;
            break;
        case J_B:
            is_crouching = true;
            break;
        case J_A:
            is_crouching = false;
            break;
        }

        // cycle through metasprite walk animations
        if ((speed_x || speed_y) && animation_delay == 0)
        {
            idx++;
            if (idx > 3)
            {
                idx = 0;
            }
            animation_delay = ANIMATION_SPEED;
        }

        if (animation_delay)
            animation_delay--;

        if (is_crouching)
        {
            move_metasprite(linda_metasprites[ANIMATION_CROUCH], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            pos_x += speed_x, pos_y += speed_y;
            if (is_facing_left)
            {
                move_metasprite_vflip(linda_metasprites[walk_animation[idx]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
            }
            else
            {
                move_metasprite(linda_metasprites[walk_animation[idx]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
            }
        }

        speed_x = 0;
        speed_y = 0;

        wait_vbl_done();
    }
}

void main(void)
{
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x16;

    while (1)
    {
        game();
    }
}