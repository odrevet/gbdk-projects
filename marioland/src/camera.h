#include <gb/gb.h>
#include <stdbool.h>
#include <stdlib.h>

#define camera_max_y ((map_height - SCREEN_HEIGHT_TILE) * TILE_SIZE)
#define camera_max_x ((map_width - SCREEN_WIDTH_TILE) * TILE_SIZE)
#define MIN(A, B) ((A) < (B) ? (A) : (B))

// current and old positions of the camera in pixels
extern uint16_t camera_x, old_camera_x;
extern uint16_t camera_x_mask;
// current and old position of the map in tiles
extern uint8_t map_pos_x, old_map_pos_x;

inline void set_camera(int map_height, int map_width, const unsigned char *map);
