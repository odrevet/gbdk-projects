#include "game_over.h"

void game_over()
{
    font_init();
    font_color(WHITE, DKGREY);
    
    font_t min_font = font_load(font_min);
    font_set(min_font);

    printf("GAME OVER\nPRESS A TO RETRY");
    
    waitpad(J_A);
}