#include "text.h"

void text_load_font() {
  set_bkg_data(TEXT_FONT_OFFSET, font_TILE_COUNT, font_tiles);
}

// Write the given char at the (x, y) position on the Background layer
void text_print_char_win(UINT8 x, UINT8 y, unsigned char chr) {
  UINT8 tile = _TEXT_CHAR_TOFU;

  if (chr >= 'a' && chr <= 'z') {
    tile = _TEXT_CHAR_A + chr - 'a';
  } else if (chr >= 'A' && chr <= 'Z') {
    tile = _TEXT_CHAR_A + chr - 'A';
  } else if (chr >= '0' && chr <= '9') {
    tile = _TEXT_CHAR_0 + chr - '0';
  } else {
    switch (chr) {
    case '\'':
      tile = _TEXT_CHAR_APOSTROPHE;
      break;
    case '-':
      tile = _TEXT_CHAR_DASH;
      break;
    case '.':
      tile = _TEXT_CHAR_DOT;
      break;
    case ',':
      tile = _TEXT_CHAR_COMMA;
      break;
    case ':':
      tile = _TEXT_CHAR_COLON;
      break;
    case '!':
      tile = _TEXT_CHAR_EXCLAMATION;
      break;
    case '?':
      tile = _TEXT_CHAR_INTERROGATION;
      break;
    case '(':
      tile = _TEXT_CHAR_LPARENTHESES;
      break;
    case ')':
      tile = _TEXT_CHAR_RPARENTHESES;
      break;
    case ' ':
      tile = _TEXT_CHAR_SPACE;
      break;
    }
  }

  set_win_tiles(x, y, 1, 1, &tile);
}

void pad(UINT8 x, UINT8 y, UINT8 offset_x, UINT8 offset_y) {
  while (offset_x < 20) {
    text_print_char_win(x + offset_x, y + offset_y, (unsigned char)' ');
    offset_x++;
  }
}

void text_print_string_win(UINT8 x, UINT8 y, unsigned char *string) {
  UINT8 offset_x = 0;
  UINT8 offset_y = 0;

  while (string[0]) {
    if (string[0] == '\n') {
      pad(x, y, offset_x, offset_y);
      offset_x = 0;
      offset_y++;
    } else {
      text_print_char_win(x + offset_x, y + offset_y, (unsigned char)string[0]);
      offset_x++;
    }

    string += 1;
  }
  pad(x, y, offset_x, offset_y);
}
