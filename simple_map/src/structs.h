#ifndef STRUCTS_H
#define STRUCTS_H

#define TILE_SIZE 8
#define OFFSET_X 8
#define OFFSET_Y 16
#define HERO_SPRITE_INDEX 0
#define MAP_WIDTH 20

typedef enum directions_e
{
    UP,
    RIGHT,
    DOWN,
    LEFT
} directions_e;

typedef enum enemy_type_e
{
    SKULL,
    GHOST,
    BAT
} enemy_type_e;

typedef struct enemy_t
{
    unsigned short x;
    unsigned short y;
    enemy_type_e type;
    int index;
} enemy_t;

#endif