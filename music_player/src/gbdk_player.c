#include <gb/gb.h>
#include "hUGEDriver.h"

extern const hUGESong_t softworld;
extern const hUGESong_t ryukenden;
extern const hUGESong_t cognition;
extern const hUGESong_t fish_n_chips;

#define NB_SONGS 4

UBYTE joy;

const unsigned char pattern1[] = {0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01};
const unsigned char pattern2[] = {0x00, 0x00, 0x7E, 0x7E, 0x40, 0x40, 0x54, 0x54, 0x48, 0x48, 0x54, 0x54, 0x40, 0x40, 0x00, 0x00};
const unsigned char map[] = {0x00, 0x20};

hUGESong_t songs[NB_SONGS];

void play_song(uint8_t index)
{
    hUGE_mute_channel(HT_CH1, 1);
    hUGE_mute_channel(HT_CH2, 1);
    hUGE_mute_channel(HT_CH3, 1);
    hUGE_mute_channel(HT_CH4, 1);

    hUGE_init(songs + index);
}

void main()
{
    songs[0] = softworld;
    songs[1] = ryukenden;
    songs[2] = cognition;
    songs[3] = fish_n_chips;

    uint8_t current_song = 0;

    LCDC_REG = 0xD1;
    BGP_REG = 0b11100100;
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    set_bkg_data(0, 1, pattern1);
    set_bkg_data(0x20, 1, pattern2);

    __critical
    {
        hUGE_init(songs + 0);
    }

    while (1)
    {

        joy = joypad();
        switch (joy)
        {
        case J_LEFT:
            play_song(--current_song);
            waitpadup();
            break;
        case J_RIGHT:
            play_song(++current_song);
            waitpadup();
            break;
        }

        hUGE_dosound();
        wait_vbl_done();
    }
}
