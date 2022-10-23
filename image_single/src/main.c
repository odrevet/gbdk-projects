#include <gb/gb.h>
#include <gb/metasprites.h>

#include "image.h"

void main(void)
{
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x8;

    set_bkg_data(0, image_TILE_COUNT, image_tiles);
    set_bkg_tiles(0, 0, image_WIDTH / 8, image_HEIGHT / 8, image_map);

    while (1)
    {
    }
}