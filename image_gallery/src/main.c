#include <gb/gb.h>
#include <gb/metasprites.h>

#include "image_list.h"

#define NB_IMAGES 4

const uint8_t TILE_COUNT[] = {image_0_TILE_COUNT, image_1_TILE_COUNT, image_2_TILE_COUNT, image_3_TILE_COUNT};
const uint8_t *TILES[] = {image_0_tiles, image_1_tiles, image_2_tiles, image_3_tiles};
const unsigned char *MAP[] = {image_0_map, image_1_map, image_2_map, image_3_map};

void load_image(uint8_t index)
{
    HIDE_BKG;
    set_bkg_data(0, TILE_COUNT[index], TILES[index]);
    set_bkg_tiles(0, 0, image_2_WIDTH / 8, image_0_HEIGHT / 8, MAP[index]);
    SHOW_BKG;
}

void main(void)
{
    DISPLAY_ON;

    uint8_t is_button_pressed = 0;
    uint8_t index = 0;
    load_image(index);

    while (1)
    {
        switch (joypad())
        {
        case J_LEFT:
            if (!is_button_pressed && index > 0)
            {
                index--;
                load_image(index);
            }
            is_button_pressed = 1;
            break;
        case J_RIGHT:
            if (!is_button_pressed && index < NB_IMAGES - 1)
            {
                index++;
                load_image(index);
            }
            is_button_pressed = 1;
            break;
        case !J_RIGHT || !J_LEFT:
            is_button_pressed = 0;
            break;
        }
    }
}