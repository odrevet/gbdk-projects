#include "camera.h"

uint16_t camera_x, camera_y, old_camera_x, old_camera_y;
uint8_t map_pos_x, map_pos_y, old_map_pos_y;
uint8_t old_map_pos_x;


inline void set_camera(int map_height, int map_width, const unsigned char *map) {
  // update hardware scroll position
  SCY_REG = camera_y;
  SCX_REG = camera_x;
  // up or down
  map_pos_y = (uint8_t)(camera_y >> 3u);
  if (map_pos_y != old_map_pos_y) {
    if (camera_y < old_camera_y) {
      set_bkg_submap(map_pos_x, map_pos_y, MIN(21u, map_width - map_pos_x), 1,
                     map, map_width);
    } else {
      if ((map_height - 18u) > map_pos_y)
        set_bkg_submap(map_pos_x, map_pos_y + 18u,
                       MIN(21u, map_width - map_pos_x), 1, map, map_width);
    }
    old_map_pos_y = map_pos_y;
  }
  // left or right
  map_pos_x = (uint8_t)(camera_x >> 3u);
  if (map_pos_x != old_map_pos_x) {
    if (camera_x < old_camera_x) {
      set_bkg_submap(map_pos_x, map_pos_y, 1, MIN(19u, map_height - map_pos_y),
                     map, map_width);
    } else {
      if ((map_width - 20u) > map_pos_x)
        set_bkg_submap(map_pos_x + 20u, map_pos_y, 1,
                       MIN(19u, map_height - map_pos_y), map, map_width);
    }
    old_map_pos_x = map_pos_x;
  }
  // set old camera position to current camera position
  old_camera_x = camera_x, old_camera_y = camera_y;
}