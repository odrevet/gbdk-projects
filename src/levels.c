#include "levels.h"

void init_level_001(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map)
{
    hero->x = TILE_SIZE * 5;
    hero->y = TILE_SIZE * 10;

    enemy->x = TILE_SIZE * 9;
    enemy->y = TILE_SIZE * 16;

    *level_number = 0;
    set_bkg_tiles(0, 0, map_001Width, map_001Height, map);

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(ENEMY_SPRITE_INDEX, enemy->x, enemy->y);
}

void init_level_002(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map)
{
    hero->x = TILE_SIZE * 2;
    hero->y = TILE_SIZE * 3;

    enemy->x = TILE_SIZE * 9;
    enemy->y = TILE_SIZE * 16;

    *level_number = 1;
    set_bkg_tiles(0, 0, map_002Width, map_002Height, map);

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(ENEMY_SPRITE_INDEX, enemy->x, enemy->y);
}

void init_level_003(hero_t *hero, enemy_t *enemy, int *level_number, unsigned char *map)
{
    hero->x = TILE_SIZE * 12;
    hero->y = TILE_SIZE * 13;

    enemy->x = TILE_SIZE * 4;
    enemy->y = TILE_SIZE * 16;

    *level_number = 2;
    set_bkg_tiles(0, 0, map_003Width, map_003Height, map);

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(ENEMY_SPRITE_INDEX, enemy->x, enemy->y);
}

inline unsigned char get_tile(unsigned short x, unsigned short y, unsigned char *map)
{
    const unsigned short index_x = (x - OFFSET_X) / TILE_SIZE;
    const unsigned short index_y = (y - OFFSET_Y) / TILE_SIZE;
    return map[MAP_WIDTH * index_y + index_x];
}

inline void set_tile(unsigned short index_x, unsigned short index_y, unsigned char value, unsigned char *map)
{
    map[MAP_WIDTH * index_y + index_x] = value;
}

bool has_collision_xy(int a_x, int a_y, int b_x, int b_y)
{
    return a_x == b_x && a_y == b_y;
}

inline bool can_move(int x, int y, unsigned char *map)
{
    return get_tile(x, y, map) != 0x01;
}

unsigned short get_possible_moves(int x, int y, unsigned char *map)
{
    unsigned short moves = 0;

    if (get_tile(x, y, map) == 0x02 && can_move(x, y - TILE_SIZE, map))
    {
        moves |= 1UL << UP;
    }

    // RIGHT
    if (can_move(x + TILE_SIZE, y, map))
    {
        moves |= 1UL << RIGHT;
    }

    // DOWN
    if (can_move(x, y + TILE_SIZE, map))
    {
        moves |= 1UL << DOWN;
    }

    // LEFT
    if (can_move(x - TILE_SIZE, y, map))
    {
        moves |= 1UL << LEFT;
    }

    return moves;
}

unsigned short get_possible_moves_bat(int x, int y, unsigned char *map)
{
    unsigned short moves = 0;

    if (can_move(x, y - TILE_SIZE, map))
    {
        moves |= 1UL << UP;
    }

    // RIGHT
    if (can_move(x + TILE_SIZE, y, map))
    {
        moves |= 1UL << RIGHT;
    }

    // DOWN
    if (can_move(x, y + TILE_SIZE, map))
    {
        moves |= 1UL << DOWN;
    }

    // LEFT
    if (can_move(x - TILE_SIZE, y, map))
    {
        moves |= 1UL << LEFT;
    }

    return moves;
}