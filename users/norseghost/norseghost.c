#include "norseghost.h"

// __attribute__((weak)) void encoder_update_user(uint8_t index, bool clockwise) {
//   if(IS_LAYER_ON(_RAISE)) { // on Raise layer control volume
//     if (clockwise){
//       tap_code(KC_MS_WH_DOWN);
//     } else{
//       tap_code(KC_MS_WH_UP);
//     }
//   } else { // on other layers emulate mouse scrollwheel
//     if (clockwise){
//       tap_code(KC_VOLU);
//     } else{
//       tap_code(KC_VOLD);
//     }
//   }
// }

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_LOWER]  = {ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP)},
    [_RAISE]  = {ENCODER_CCW_CW(LSG(KC_RIGHT), LSG(KC_LEFT))},
    [_QWERTY] = {ENCODER_CCW_CW(KC_VOLU, KC_VOLD)},
    [_ADJUST] = {ENCODER_CCW_CW(RGB_RMOD, RGB_MOD)},
};
#endif

__attribute__((weak)) bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
        case GAME:
            if (record->event.pressed) {
                layer_on(_GAME);
            } else {
                layer_off(_GAME);
            }
            return false;
            break;
    }
    return true;
}
