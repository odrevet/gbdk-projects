#include <gb/gb.h>
#include <gb/metasprites.h>

#include <stdint.h>
#include <stdbool.h>

#include "graphics/linda.h"
#include "graphics/city_data.h"
#include "graphics/lindagb.h"
#include "maps/map_001.h"

#include "hUGEDriver.h"

extern const hUGESong_t softworld;
extern const hUGESong_t a_sad_touch;

#define HERO_SPRITE_INDEX 0
#define SPR_NUM_START 0
#define JUMP_MAX 32
#define SPEED 12

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
uint8_t is_crouching = false;
uint8_t is_jumping = false;
uint8_t is_facing_left = false;
bool freeze_movement = false;
uint8_t jump_current = 0;

void draw_linda(void)
{
    if (is_jumping || jump_current > 0)
    {
        if (is_facing_left)
        {
            move_metasprite_vflip(linda_metasprites[ANIMATION_JUMP], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            move_metasprite(linda_metasprites[ANIMATION_JUMP], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
    else if (is_crouching)
    {
        if (is_facing_left)
        {
            move_metasprite_vflip(linda_metasprites[ANIMATION_CROUCH], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            move_metasprite(linda_metasprites[ANIMATION_CROUCH], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
    else
    {
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

        if (is_facing_left)
        {
            move_metasprite_vflip(linda_metasprites[walk_animation[idx]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            move_metasprite(linda_metasprites[walk_animation[idx]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
}

int game(void)
{

    pos_x = pos_y = 96 << 4;
    speed_x = speed_y = 0;
    idx = 0;
    animation_delay = 0;

    // Intro screen
    set_bkg_data(0, lindagb_TILE_COUNT, lindagb_tiles);
    set_bkg_tiles(0, 0, lindagb_WIDTH / 8, lindagb_HEIGHT / 8, lindagb_map);

    hUGE_init(&softworld);
    bool done = false;
    while (!done)
    {
        switch (joypad())
        {
        case J_START:
        case J_A:
        case J_B:
            done = true;
            break;
        }

        hUGE_dosound();
        wait_vbl_done();
    }

    // Level screen
    hUGE_mute_channel(HT_CH1, 1);
    hUGE_mute_channel(HT_CH2, 1);
    hUGE_mute_channel(HT_CH3, 1);
    hUGE_mute_channel(HT_CH4, 1);
    hUGE_init(&a_sad_touch);

    set_bkg_data(0, 16, city_data);
    set_bkg_submap(0, 0, map_001Width, map_001Height, map_001, map_001Width);
    set_sprite_data(HERO_SPRITE_INDEX, sizeof(linda_tiles) >> 4, linda_tiles);

    while (1)
    {

        switch (joypad())
        {
        case J_LEFT:
            if (!freeze_movement)
            {
                speed_x -= SPEED;
                is_facing_left = true;
            }
            break;
        case J_RIGHT:
            if (!freeze_movement)
            {
                speed_x += SPEED;
                is_facing_left = false;
            }
            break;
        case J_DOWN:
            if (!freeze_movement && pos_y + speed_y < 144 << 4)

            {
                speed_y += SPEED;
            }
            break;
        case J_UP:
            if (!freeze_movement && pos_y + speed_y > 74 << 4)
            {
                speed_y -= SPEED;
            }
            break;
        case (J_A | J_B):
            if ((is_jumping == false || jump_current == 0) && !freeze_movement)
            {
                is_jumping = true;
            }
            break;
        }

        freeze_movement = is_crouching || is_jumping || jump_current > 0;

        draw_linda();

        if (is_jumping)
        {

            if (jump_current < JUMP_MAX)
            {
                speed_y -= SPEED;
                if (is_facing_left)
                {
                    speed_x -= SPEED;
                }
                else
                {
                    speed_x += SPEED;
                }
                jump_current++;
            }
            else
            {
                is_jumping = false;
            }
        }
        else
        {
            // gravity
            if (jump_current > 0)
            {
                jump_current--;
                speed_y += SPEED;
                if (is_facing_left)
                {
                    speed_x -= SPEED;
                }
                else
                {
                    speed_x += SPEED;
                }
            }
        }

        if (pos_x + speed_x > 100 << 4)
        {
            speed_x = 0;
            scroll_bkg(1, 0);
        }
        else if (pos_x + speed_x > 16 << 4)
        {
            pos_x += speed_x;
        }

        pos_y += speed_y;

        speed_x = 0;
        speed_y = 0;

        hUGE_dosound();
        wait_vbl_done();
    }
}

void main(void)
{
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x16;

    while (1)
    {
        game();
    }
}