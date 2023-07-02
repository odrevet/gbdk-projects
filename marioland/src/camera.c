#include "camera.h"

uint16_t camera_x, old_camera_x;
uint8_t map_pos_x, old_map_pos_x;

inline void set_camera(int map_height, int map_width,
                       const unsigned char *map) {
  SCX_REG = camera_x;
  map_pos_x = (uint8_t)(camera_x >> 3u);
  if (map_pos_x != old_map_pos_x && map_width - 20u > map_pos_x) {
    set_bkg_submap(map_pos_x + 20u, 0, 1, MIN(19u, map_height), map, map_width);
    old_map_pos_x = map_pos_x;
  }
  old_camera_x = camera_x;
}