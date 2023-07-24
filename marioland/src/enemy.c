#include "enemy.h"

uint8_t enemy_count = 0;
enemy_t enemies[ENEMY_MAX];

void enemy_new(uint16_t x, uint16_t y) {
  if (enemy_count < ENEMY_MAX) {
    enemy_t enemy = {.x = x, .y = y, .vel_x = 0, .vel_y = 0};
    enemies[enemy_count] = enemy;
    enemy_count++;
  }
}

void enemy_update() {
  for (uint8_t index_enemy = 0; index_enemy < enemy_count; index_enemy++) {
    enemies[index_enemy].x--;
  }
}

void enemy_draw() {
  for (int index_enemy = 0; index_enemy < enemy_count; index_enemy++) {
    int enemy_draw_x_camera_offset = enemies[index_enemy].x - camera_x;
    metasprite_t *enemy_metasprite = enemies_metasprites[0];
    move_metasprite(enemy_metasprite, SPRITE_START_ENEMIES,
                    SPRITE_START_ENEMIES, enemy_draw_x_camera_offset,
                    enemies[index_enemy].y);
  }
}