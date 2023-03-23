#include "norseghost.h"

/*
 * The `LAYOUT_atreus_base` macro is a template to allow the use of identical
 * modifiers for the default layouts (e.g. QWERTY, Colemak, Dvorak, etc), so
 * that there is no need to set them up for each layout, and modify all of
 * them if I want to change them.  This helps to keep consistency and ease
 * of use. K## is a placeholder to pass through the individual keycodes
 */

// clang-format off

#define LAYOUT_atreus_base( \
    K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, \
    K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, \
    K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A  \
  ) \
  LAYOUT_wrapper( \
     K01,    K02,     K03,     K04,    K05,                           K06,     K07,     K08,    K09,      K0A,    \
     K11,    K12,     K13,     K14,    K15,                           K16,     K17,     K18,    K19,      K1A,    \
     K21,    K22,     K23,     K24,    K25,     TD(LOWER), TD(RAISE), K26,     K27,     K28,    K29,      K2A,    \
     KC_ESC, KC_TAB,  TD(WM),  KC_DEL, SYM_BKSP, KC_SPC,    SYM_ENT,   KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, KC_AE   \
     )
#define LAYOUT_atreus_base_wrapper(...)       LAYOUT_atreus_base(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT_atreus_base_wrapper(
    _________________QWERTY_L1_________________, _________________QWERTY_R1_________________,
    _________________QWERTY_L2_________________, _________________QWERTY_R2_________________,
    _________________QWERTY_L3_________________, _________________QWERTY_R3_________________
  ),

  [_GAME]  = LAYOUT_atreus_base_wrapper(
    _________________GAME_L1___________________, _________________GAME_R1___________________,
    _________________GAME_L2___________________, _________________GAME_R2___________________,
    _________________GAME_L3___________________, _________________GAME_R3___________________
  ),

  [_SYMBOLS] = LAYOUT_atreus_base_wrapper(
    _________________SYMBOLS_L1________________, _________________SYMBOLS_R1________________,
    _________________SYMBOLS_L2________________, _________________SYMBOLS_R2________________,
    _________________SYMBOLS_L3________________, _________________SYMBOLS_R3________________
  ),

  [_LOWER] = LAYOUT_atreus_base_wrapper(
    _________________LOWER_L1__________________, _________________LOWER_R1__________________,
    _________________LOWER_L2__________________, _________________LOWER_R2__________________,
    _________________LOWER_L3__________________, _________________LOWER_R3__________________
  ),

  [_RAISE] = LAYOUT_atreus_base_wrapper(
    _________________RAISE_L1__________________, _________________RAISE_R1__________________,
    _________________RAISE_L2__________________, _________________RAISE_R2__________________,
    _________________RAISE_L3__________________, _________________RAISE_R3__________________
  ),

  [_NAV] = LAYOUT_atreus_base_wrapper(
    ___________________BLANK___________________, ___________________BLANK___________________,
    ___________________BLANK___________________, ___________________NAV_1___________________,
    ___________________BLANK___________________, ___________________NAV_2___________________
  )
};
static bool is_alpha_key(keypos_t key) {
    return key.row <= 2 && (key.col <= 4 || key.col >= 7);
}
