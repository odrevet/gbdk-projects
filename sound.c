#include "sound.h"

void sound_init()
{
    NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
}

void move_sound()
{
    NR10_REG = 0x16;
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0x00;
    NR14_REG = 0xC3;
}

void error_sound()
{
    NR10_REG = 0x76;
    NR11_REG = 0x40;
    NR12_REG = 0x73;
    NR13_REG = 0xA0;
    NR14_REG = 0xC3;
}