#include <gb/gb.h>
#include <gb/metasprites.h>

#include "image_list.h"

void load_image(uint8_t index)
{
    switch_bank(index);
    set_bkg_data(0, TILE_COUNT[index], TILES[index]);
    set_bkg_tiles(0, 0, image_0_WIDTH / 8, image_0_HEIGHT / 8, MAP[index]);
}

void main(void)
{
    DISPLAY_ON;
    SHOW_BKG;

    uint8_t index = 0;
    load_image(index);

    while (1)
    {
        switch (joypad())
        {
        case J_LEFT:
            if (index > 0)
            {
                load_image(--index);
            }
            waitpadup();
            break;
        case J_RIGHT:
            if (index < NB_IMAGES - 1)
            {
                load_image(++index);
            }
            waitpadup();
            break;
        }
    }
}