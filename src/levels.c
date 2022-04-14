#include "levels.h"

void init_level_001(hero_t *hero, enemy_t *enemy, int *enemy_count, int *level_number, unsigned char *map)
{
    // hero
    hero->x = TILE_SIZE * 5;
    hero->y = TILE_SIZE * 10;

    // enemy
    *enemy_count = 1;
    enemy[0].type = SKULL;
    enemy[0].x = TILE_SIZE * 8;
    enemy[0].y = TILE_SIZE * 16;
    enemy[0].index = 1;

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(enemy[0].index, enemy[0].x, enemy[0].y);
    hide_sprite(enemy[1].index);

    set_sprite_tile(enemy[0].index, 4);

    // level
    *level_number = 0;
    set_bkg_tiles(0, 0, map_001Width, map_001Height, map);
}

void init_level_002(hero_t *hero, enemy_t *enemy, int *enemy_count, int *level_number, unsigned char *map)
{
    // hero
    hero->x = TILE_SIZE * 2;
    hero->y = TILE_SIZE * 3;

    // enemy
    *enemy_count = 1;
    enemy[0].type = SKULL;
    enemy[0].x = TILE_SIZE * 9;
    enemy[0].y = TILE_SIZE * 16;

    move_sprite(HERO_SPRITE_INDEX, hero->x, hero->y);
    move_sprite(enemy[0].index, enemy->x, enemy->y);
    hide_sprite(enemy[1].index);

    set_sprite_tile(enemy[0].index, 4);

    // level
    *level_number = 1;
    set_bkg_tiles(0, 0, map_002Width, map_002Height, map);
}

void init_level_003(hero_t *hero, enemy_t *enemy, int *enemy_count, int *level_number, unsigned char *map)
{
    // hero
    hero->x = TILE_SIZE * 12;
    hero->y = TILE_SIZE * 13;

    // enemy
    *enemy_count = 2;
    enemy[0].type = SKULL;
    enemy[0].x = TILE_SIZE * 4;
    enemy[0].y = TILE_SIZE * 16;

    enemy[1].type = BAT;
    enemy[1].x = TILE_SIZE * 2;
    enemy[1].y = TILE_SIZE * 3;
    enemy[1].index = 2;

    move_sprite(HERO_SPRITE_INDEX, hero[0].x, hero[0].y);
    move_sprite(enemy[0].index, enemy[0].x, enemy[0].y);
    move_sprite(enemy[1].index, enemy[1].x, enemy[1].y);

    set_sprite_tile(enemy[0].index, 4);
    set_sprite_tile(enemy[1].index, 2);

    // level
    *level_number = 2;
    set_bkg_tiles(0, 0, map_003Width, map_003Height, map);
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