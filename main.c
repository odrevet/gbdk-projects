#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>

#include "graphics/level_bg.h"
#include "graphics/hero_tiles.h"
#include "maps/map_001.h"
#include "maps/map_002.h"

#include "game_over.h"

#define TILE_SIZE 8
#define OFFSET_X 8
#define OFFSET_Y 16
#define HERO_SPRITE_INDEX 0
#define ENEMY_SPRITE_INDEX 1
#define MAP_WIDTH 20

int level_number = 0;
unsigned char *maps[] = {map_001, map_002};
#define current_map maps[level_number]

typedef enum directions_e
{
    UP,
    RIGHT,
    DOWN,
    LEFT
} directions_e;

typedef enum enemy_type_e
{
    DOPPLEDANGER,
    GHOST,
    BAT
} enemy_type_e;

typedef struct hero_t
{
    unsigned short x;
    unsigned short y;
    unsigned short tile_index;
} hero_t;

typedef struct enemy_t
{
    unsigned short x;
    unsigned short y;
    enemy_type_e type;
} enemy_t;

inline unsigned char get_tile(unsigned short x, unsigned short y)
{
    const unsigned short index_x = (x - OFFSET_X) / TILE_SIZE;
    const unsigned short index_y = (y - OFFSET_Y) / TILE_SIZE;
    return current_map[MAP_WIDTH * index_y + index_x];
}

inline void set_tile(unsigned short index_x, unsigned short index_y, unsigned char value)
{
    current_map[MAP_WIDTH * index_y + index_x] = value;
}

bool has_collision(hero_t *hero, enemy_t *ennemy)
{
    return hero->x == ennemy->x && hero->y == ennemy->y;
}

bool has_collision_xy(int a_x, int a_y, int b_x, int b_y)
{
    return a_x == b_x && a_y == b_y;
}

inline bool can_move(int x, int y)
{
    return get_tile(x, y) != 0x01;
}

unsigned short get_possible_moves(int x, int y)
{
    unsigned short moves = 0;

    if (get_tile(x, y) == 0x02 && can_move(x, y - TILE_SIZE))
    {
        moves |= 1UL << UP;
    }

    // RIGHT
    if (can_move(x + TILE_SIZE, y))
    {
        moves |= 1UL << RIGHT;
    }

    // DOWN
    if (can_move(x, y + TILE_SIZE))
    {
        moves |= 1UL << DOWN;
    }

    // LEFT
    if (can_move(x - TILE_SIZE, y))
    {
        moves |= 1UL << LEFT;
    }

    return moves;
}

void enemy_move(enemy_t *enemy, hero_t *hero)
{
    int possible_moves = get_possible_moves(enemy->x, enemy->y);

    // Gravity
    if (get_tile(enemy->x, enemy->y + TILE_SIZE) == 0x00)
    {
        enemy->y += TILE_SIZE;
        scroll_sprite(ENEMY_SPRITE_INDEX, 0, TILE_SIZE);
    }
    else
    {
        int bit = (possible_moves >> LEFT) & 1U;
        if ((possible_moves >> LEFT) & 1U && hero->x < enemy->x)
        {
            enemy->x -= TILE_SIZE;
            scroll_sprite(ENEMY_SPRITE_INDEX, -TILE_SIZE, 0);
        }
        else if ((possible_moves >> RIGHT) & 1U && hero->x >= enemy->x)
        {
            enemy->x += TILE_SIZE;
            scroll_sprite(ENEMY_SPRITE_INDEX, +TILE_SIZE, 0);
        }
    }
}

void move_sound()
{
    NR10_REG = 0x16;
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;
}

void error_sound()
{
    NR10_REG = 0x76;
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0xA0;
    NR14_REG = 0xC3;
}

void init_level_001(hero_t *hero, enemy_t *enemy)
{
    hero->x = TILE_SIZE * 5;
    hero->y = TILE_SIZE * 10;

    enemy->x = TILE_SIZE * 9;
    enemy->y = TILE_SIZE * 16;

    level_number = 0;
    set_bkg_tiles(0, 0, map_001Width, map_001Height, current_map);

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(ENEMY_SPRITE_INDEX, enemy->x, enemy->y);
}

void init_level_002(hero_t *hero, enemy_t *enemy)
{
    hero->x = TILE_SIZE * 2;
    hero->y = TILE_SIZE * 3;

    enemy->x = TILE_SIZE * 9;
    enemy->y = TILE_SIZE * 16;

    level_number = 1;
    set_bkg_tiles(0, 0, map_002Width, map_002Height, current_map);

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(ENEMY_SPRITE_INDEX, enemy->x, enemy->y);
}

bool on_hero_move(hero_t *hero, enemy_t *enemy)
{
    // update hero tile
    hero->tile_index = (++hero->tile_index) % 2;
    set_sprite_tile(0, hero->tile_index);

    enemy_move(enemy, hero);
    move_sound();
    if (has_collision(hero, enemy))
    {
        game_over();
        return true;
    }

    // check if exit reached
    if (get_tile(hero->x, hero->y) == 0x03)
    {
        level_number++;
        init_level_002(hero, enemy);
        return false;
    }

    delay(150);

    return false;
}

int game(void)
{
    hero_t hero;
    hero.tile_index = 0;

    enemy_t enemy;
    enemy.type = DOPPLEDANGER;

    init_level_001(&hero, &enemy);

    // init level graphics
    set_bkg_data(0, 4, level_bg);

    // init hero graphics
    set_sprite_data(0, 2, hero_tiles);
    set_sprite_tile(0, hero.tile_index);

    while (1)
    {
        // gravity
        if (get_tile(hero.x, hero.y + TILE_SIZE) == 0x00)
        {
            hero.y += TILE_SIZE;
            scroll_sprite(HERO_SPRITE_INDEX, 0, TILE_SIZE);

            if (on_hero_move(&hero, &enemy))
            {
                return 0;
            }
        }

        switch (joypad())
        {
        case J_LEFT:

            if (can_move(hero.x - TILE_SIZE, hero.y))
            {
                hero.x -= TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, -TILE_SIZE, 0);

                if (on_hero_move(&hero, &enemy))
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

            if (can_move(hero.x + TILE_SIZE, hero.y))
            {
                hero.x += TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, TILE_SIZE, 0);

                if (on_hero_move(&hero, &enemy))
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

            if (can_move(hero.x, hero.y + TILE_SIZE))
            {
                hero.y += TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, 0, TILE_SIZE);

                if (on_hero_move(&hero, &enemy))
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

            if (get_tile(hero.x, hero.y) == 0x02 && can_move(hero.x, hero.y - TILE_SIZE))
            {
                hero.y -= TILE_SIZE;
                scroll_sprite(HERO_SPRITE_INDEX, 0, -TILE_SIZE);

                if (on_hero_move(&hero, &enemy))
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
    // init sound
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    while (1)
    {
        game();
    }
}