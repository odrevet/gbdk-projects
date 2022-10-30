//AUTOGENERATED FILE FROM png2asset

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

BANKREF(city)

const palette_color_t city_palettes[8] = {
	RGB8(236,236,236), RGB8(198,198,198), RGB8(150,150,150), RGB8( 84, 84, 84)
	,
	RGB8(197,197,197), RGB8( 84, 84, 84), RGB8(  0,  0,  0), RGB8(  0,  0,  0)
	
};

const uint8_t city_tiles[64] = {
	0xff,0x00,0x00,0x80,
	0x3f,0x80,0x3f,0x80,
	0x3f,0x80,0x3f,0x80,
	0x3f,0x80,0x00,0xff,
	
0x00,0x00,0x7f,0x00,
	0x7f,0x00,0x00,0x00,
	0xfd,0x00,0xfd,0x00,
	0x00,0x00,0x7f,0x00,
	
0x00,0xff,0x00,0xff,
	0x00,0xff,0x00,0xff,
	0x00,0xff,0x00,0xff,
	0x00,0xff,0x00,0xff,
	
0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
	
};

const metasprite_t city_metasprite0[] = {
	METASPR_ITEM(-4, -4, 0, 0), METASPR_TERM
};

const metasprite_t city_metasprite1[] = {
	METASPR_ITEM(-4, -4, 1, 1), METASPR_TERM
};

const metasprite_t city_metasprite2[] = {
	METASPR_ITEM(-4, -4, 2, 0), METASPR_TERM
};

const metasprite_t city_metasprite3[] = {
	METASPR_ITEM(-4, -4, 3, 0), METASPR_TERM
};

const metasprite_t* const city_metasprites[4] = {
	city_metasprite0, city_metasprite1, city_metasprite2, city_metasprite3
};
