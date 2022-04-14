#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#include "graphics/level_bg.h"
#include "graphics/characters_tiles.h"

#include "structs.h"
#include "hero.h"
#include "enemy.h"
#include "game_over.h"
#include "levels.h"
#include "sound.h"

int level_number = 0;
unsigned char *maps[] = {map_001, map_002, map_003};
#define current_map maps[level_number]

bool has_collision(hero_t *hero, enemy_t *ennemy)
{
    return hero->x == ennemy->x && hero->y == ennemy->y;
}

void on_exit_reached(hero_t *hero, enemy_t *enemy, int *enemy_count)
{
    level_number++;
    if (level_number == 1)
    {
        init_level_002(hero, enemy, enemy_count, &level_number, current_map);
    }
    else
    {
        init_level_003(hero, enemy, enemy_count, &level_number, current_map);
    }
}

bool on_hero_move(hero_t *hero, enemy_t enemy[], int enemy_count)
{
    // update hero tile
    hero->tile_index = (++hero->tile_index) % 2;
    set_sprite_tile(HERO_SPRITE_INDEX, hero->tile_index);

    for (int index = 0; index < enemy_count; index++)
    {
        if (enemy[index].type == BAT)
        {
            enemy_move_bat(enemy + index, hero, current_map);
        }
        else
        {
            enemy_move(enemy + index, hero, current_map);
        }

        if (has_collision(hero, enemy + index))
        {
            game_over();
            return true;
        }
    }

    move_sound();

    // check if exit reached
    if (get_tile(hero->x, hero->y, current_map) == 0x03)
    {
        on_exit_reached(hero, enemy, enemy_count);
        return false;
    }

    delay(150);

    return false;
}

int game(void)
{
    hero_t hero;
    hero.tile_index = 0;

    enemy_t enemy[2];
    level_number = 0;
    int enemy_count;
    init_level_001(&hero, enemy, &enemy_count, &level_number, current_map);

    // init level graphics
    set_bkg_data(0, 4, level_bg);

    // init sprite graphics
    set_sprite_data(0, 5, characters_tiles);
    // set hero graphics
    set_sprite_tile(HERO_SPRITE_INDEX, hero.tile_index);

    while (1)
    {
        // gravity
        if (get_tile(hero.x, hero.y + TILE_SIZE, current_map) == 0x00)
        {
            hero.y += TILE_SIZE;
            scroll_sprite(HERO_SPRITE_INDEX, 0, TILE_SIZE);

            if (on_hero_move(&hero, enemy, enemy_count))
            {
                return 0;
            }
        }

        switch (joypad())
        {
        case J_LEFT:

            if (can_move(hero.x - TILE_SIZE, hero.y, current_map))
            {
                hero.x -= TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, -TILE_SIZE, 0);
                set_sprite_prop(HERO_SPRITE_INDEX, S_FLIPX);

                if (on_hero_move(&hero, enemy, enemy_count))
                {
                    return 0;
                }
            }
            else
            {
                error_sound();
            }
            break;

        case J_RIGHT:

            if (can_move(hero.x + TILE_SIZE, hero.y, current_map))
            {
                hero.x += TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, TILE_SIZE, 0);
                set_sprite_prop(HERO_SPRITE_INDEX, get_sprite_prop(HERO_SPRITE_INDEX) & ~S_FLIPX);

                if (on_hero_move(&hero, enemy, enemy_count))
                {
                    return 0;
                }
            }
            else
            {
                error_sound();
            }
            break;

        case J_DOWN:

            if (can_move(hero.x, hero.y + TILE_SIZE, current_map))
            {
                hero.y += TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, 0, TILE_SIZE);

                if (on_hero_move(&hero, enemy, enemy_count))
                {
                    return 0;
                }
            }
            else
            {
                error_sound();
            }
            break;

        case J_UP:

            if (get_tile(hero.x, hero.y, current_map) == 0x02 && can_move(hero.x, hero.y - TILE_SIZE, current_map))
            {
                hero.y -= TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, 0, -TILE_SIZE);

                if (on_hero_move(&hero, enemy, enemy_count))
                {
                    return 0;
                }
            }
            else
            {
                error_sound();
            }
            break;
        }

        wait_vbl_done();
    }
}

void main(void)
{
    sound_init();

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    while (1)
    {
        game();
    }
}