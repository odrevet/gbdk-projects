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
#define PUNCH_DURATION 32

uint16_t pos_x, pos_y;
int16_t speed_x, speed_y;

uint8_t animation_delay;

enum ANIMATION_INDEX
{
    ANIMATION_STILL = 0,
    ANIMATION_WALK_0,
    ANIMATION_WALK_1,
    ANIMATION_JUMP,
    ANIMATION_CROUCH,
    ANIMATION_CLIMB,
    ANIMATION_JUMP_KICK,
    ANIMATION_FALL,
    ANIMATION_PUNCH_1,
    ANIMATION_PUNCH_2,
    ANIMATION_HIT_1,
    ANIMATION_HIT_2,
    ANIMATION_HIT_3,
    ANIMATION_HIT_4,
    ANIMATION_ON_GROUND,
    ANIMATION_GRABBED
};

uint8_t walk_animation_index;
uint8_t punch_animation_index;
uint8_t walk_animation[] = {ANIMATION_STILL, ANIMATION_WALK_0, ANIMATION_STILL, ANIMATION_WALK_1};
uint8_t punch_animation[] = {ANIMATION_PUNCH_1, ANIMATION_PUNCH_2};

const uint8_t ANIMATION_SPEED = 8;
uint8_t is_crouching = false;
uint8_t is_jumping = false;
uint8_t is_punching = false;
uint8_t is_facing_left = false;
bool freeze_movement = false;
uint8_t jump_current = 0;
bool j_a_released = false;
uint8_t punch_current = 0;

joypads_t joypads;

void set_song(const hUGESong_t *song)
{
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;
    hUGE_init(song);
}

void draw_linda(void)
{
    uint8_t hw_sprites = 0;

    if (is_jumping || jump_current > 0)
    {
        if (is_facing_left)
        {
            hw_sprites = move_metasprite_vflip(linda_metasprites[ANIMATION_JUMP_KICK], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            hw_sprites = move_metasprite(linda_metasprites[ANIMATION_JUMP_KICK], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
    else if (is_punching)
    {
        if (is_facing_left)
        {
            hw_sprites = move_metasprite_vflip(linda_metasprites[punch_animation[punch_animation_index]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            hw_sprites = move_metasprite(linda_metasprites[punch_animation[punch_animation_index]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
    else if (is_crouching)
    {
        if (is_facing_left)
        {
            hw_sprites = move_metasprite_vflip(linda_metasprites[ANIMATION_CROUCH], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            hw_sprites = move_metasprite(linda_metasprites[ANIMATION_CROUCH], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }
    else
    {
        // cycle through metasprite walk animations
        if ((speed_x || speed_y) && animation_delay == 0)
        {
            walk_animation_index++;
            if (walk_animation_index > 3)
            {
                walk_animation_index = 0;
            }
            animation_delay = ANIMATION_SPEED;
        }

        if (animation_delay)
            animation_delay--;

        if (is_facing_left)
        {
            hw_sprites = move_metasprite_vflip(linda_metasprites[walk_animation[walk_animation_index]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
        else
        {
            hw_sprites = move_metasprite(linda_metasprites[walk_animation[walk_animation_index]], HERO_SPRITE_INDEX, SPR_NUM_START, (pos_x >> 4), (pos_y >> 4));
        }
    }

    hide_sprites_range(hw_sprites, MAX_HARDWARE_SPRITES);
}

int game()
{

    pos_x = pos_y = 96 << 4;
    speed_x = speed_y = 0;
    walk_animation_index = 0;
    punch_animation_index = 0;
    animation_delay = 0;

    // Intro screen
    set_bkg_data(0, lindagb_TILE_COUNT, lindagb_tiles);
    set_bkg_tiles(0, 0, lindagb_WIDTH / 8, lindagb_HEIGHT / 8, lindagb_map);

    set_song(&softworld);

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
    set_song(&a_sad_touch);

    set_bkg_data(0, 16, city_data);
    set_bkg_submap(0, 0, map_001Width, map_001Height, map_001, map_001Width);
    set_sprite_data(HERO_SPRITE_INDEX, sizeof(linda_tiles) >> 4, linda_tiles);

    while (1)
    {
        joypad_ex(&joypads);

        if (joypads.joy0 & J_LEFT)
        {
            if (!freeze_movement)
            {
                speed_x -= SPEED;
                is_facing_left = true;
            }
        }

        if (joypads.joy0 & J_RIGHT)
        {
            if (!freeze_movement)
            {
                speed_x += SPEED;
                is_facing_left = false;
            }
        }

        if (joypads.joy0 & J_DOWN)
        {
            if (!freeze_movement && pos_y + speed_y < 144 << 4)

            {
                speed_y += SPEED;
            }
        }

        if (joypads.joy0 & J_UP)
        {
            if (!freeze_movement && pos_y + speed_y > 74 << 4)
            {
                speed_y -= SPEED;
            }
        }

        if (joypads.joy0 & J_A && joypads.joy0 & J_B)
        {
            if ((is_jumping == false || jump_current == 0) && !freeze_movement)
            {
                is_jumping = true;
            }
        }
        
        if (joypads.joy0 & J_A)
        {
            if (is_punching == false && !freeze_movement && j_a_released == true)
            {
                is_punching = true;
            }
            j_a_released = false;
        }
        else
        {
            j_a_released = true;
        }



        freeze_movement = is_crouching || is_jumping || jump_current > 0 || is_punching;

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

        if (++punch_current > PUNCH_DURATION)
        {
            is_punching = false;
            punch_current = 0;
        }

        hUGE_dosound();
        wait_vbl_done();
    }
}

void main(void)
{

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x16;

    joypad_init(1, &joypads);

    while (1)
    {
        game();
    }
}