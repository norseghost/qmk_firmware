
#include "norseghost.h"

/*
 * The `LAYOUT_atreus_base` macro is a template to allow the use of identical
 * modifiers for the default layouts (eg QWERTY, Colemak, Dvorak, etc), so
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
     K01,    K02,     K03,     K04,      K05,                         K06,      K07,     K08,     K09,      K0A,    \
     K11,    K12,     K13,     K14,      K15,                         K16,      K17,     K18,     K19,      K1A,    \
     K21,    K22,     K23,     K24,      K25,      LOWER,    RAISE,   K26,      K27,     K28,     K29,      K2A,    \
     KC_ESC, KC_TAB,  KC_LALT, KC_DEL,   SYM_BKSP, KC_SPC,   KC_SPC,  SYM_ENT,  KC_RALT, KC_LEFT, KC_RIGHT, KC_DOWN \
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

  [_COLEMAK] = LAYOUT_atreus_base_wrapper(
    _________________COLEMAK_L1________________, _________________COLEMAK_R1________________,
    _________________COLEMAK_L2________________, _________________COLEMAK_R2________________,
    _________________COLEMAK_L3________________, _________________COLEMAK_R3________________
  ),

  [_DVORAK] = LAYOUT_atreus_base_wrapper(
    _________________DVORAK_L1_________________, _________________DVORAK_R1_________________,
    _________________DVORAK_L2_________________, _________________DVORAK_R2_________________,
    _________________DVORAK_L3_________________, _________________DVORAK_R3_________________
  ),

  [_WORKMAN] = LAYOUT_atreus_base_wrapper(
    _________________WORKMAN_L1________________, _________________WORKMAN_R1________________,
    _________________WORKMAN_L2________________, _________________WORKMAN_R2________________,
    _________________WORKMAN_L3________________, _________________WORKMAN_R3________________
  ),

  [_NORMAN] = LAYOUT_atreus_base_wrapper(
    _________________NORMAN_L1_________________, _________________NORMAN_L1_________________,
    _________________NORMAN_L2_________________, _________________NORMAN_R2_________________,
    _________________NORMAN_L3_________________, _________________NORMAN_R3_________________
  ),

  [_MALTRON] = LAYOUT_atreus_base_wrapper(
    _________________MALTRON_L1________________, _________________MALTRON_R1________________,
    _________________MALTRON_L2________________, _________________MALTRON_R2________________,
    _________________MALTRON_L3________________, _________________MALTRON_R3________________
  ),

  [_EUCALYN] = LAYOUT_atreus_base_wrapper(
    _________________EUCALYN_L1________________, _________________EUCALYN_R1________________,
    _________________EUCALYN_L2________________, _________________EUCALYN_R2________________,
    _________________EUCALYN_L3________________, _________________EUCALYN_R3________________
  ),

  [_CARPLAX] = LAYOUT_atreus_base_wrapper(
    _____________CARPLAX_QFMLWY_L1_____________, _____________CARPLAX_QFMLWY_R1_____________,
    _____________CARPLAX_QFMLWY_L2_____________, _____________CARPLAX_QFMLWY_R2_____________,
    _____________CARPLAX_QFMLWY_L3_____________, _____________CARPLAX_QFMLWY_R3_____________
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
