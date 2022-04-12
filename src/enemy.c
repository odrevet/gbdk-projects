#include "enemy.h"

void enemy_move(enemy_t *enemy, hero_t *hero, unsigned char *map)
{
    int possible_moves = get_possible_moves(enemy->x, enemy->y, map);

    // Gravity
    if (get_tile(enemy->x, enemy->y + TILE_SIZE, map) == 0x00)
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

void enemy_move_bat(enemy_t *enemy, hero_t *hero, unsigned char *map)
{
    int possible_moves = get_possible_moves_bat(enemy->x, enemy->y, map);

    int bit = (possible_moves >> LEFT) & 1U;
    if ((possible_moves >> LEFT) & 1U && hero->x < enemy->x)
    {
        enemy->x -= TILE_SIZE;
        scroll_sprite(ENEMY_SPRITE_INDEX, -TILE_SIZE, 0);
    }
    else if ((possible_moves >> RIGHT) & 1U && hero->x >= enemy->x)
    {
        enemy->x += TILE_SIZE;
        scroll_sprite(ENEMY_SPRITE_INDEX, TILE_SIZE, 0);
    }

    possible_moves = get_possible_moves_bat(enemy->x, enemy->y, map);
    if ((possible_moves >> UP) & 1U && hero->y < enemy->y)
    {
        enemy->y -= TILE_SIZE;
        scroll_sprite(ENEMY_SPRITE_INDEX, 0, -TILE_SIZE);
    }
    else if ((possible_moves >> DOWN) & 1U && hero->y >= enemy->y)
    {
        enemy->y += TILE_SIZE;
        scroll_sprite(ENEMY_SPRITE_INDEX, 0, TILE_SIZE);
    }
}
