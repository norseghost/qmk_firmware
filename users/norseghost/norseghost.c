#include "norseghost.h"
#include "quantum.h"
#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#    include "secrets.h"
#endif

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
// clang-format off
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [_NAV] = {ENCODER_CCW_CW(KC_PGDN, KC_PGUP)},
    [_SYMBOLS] = {ENCODER_CCW_CW(KC_NO, KC_NO)},
    [_LOWER] = {ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP)},
    [_RAISE] = {ENCODER_CCW_CW(LSG(KC_RIGHT), LSG(KC_LEFT))},
    [_GAME] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
#endif
#include "features/achordion.h"
bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Exceptionally consider the following chords as holds, even though they
    // are on the same hand in Dvorak.
    switch (tap_hold_keycode) {
        /* case TD(LOWER): */
        /* case RAISE: */
        /*     return true; */
        /*     break; */
        /**/
        /* case HOME_S:  // S + H and S + G. */
        /*   if (other_keycode == HOME_H || other_keycode == KC_G) { return true; } */
        /*   break; */
    }
    // Also allow same-hand holds when the other key is in the rows below the
    // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
    /* if (other_record->event.key.row == 3) { */
    /*     return true; */
    /* } */
    switch (other_keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            other_keycode &= 0xff; // Get base keycode.
    }
    // Allow same-hand holds with non-alpha keys.
    if (other_keycode > KC_Z) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

// clang-format on
__attribute__((weak)) bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    return true;
}

__attribute__((weak)) bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_achordion(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        // case LOWER:
        //     if (record->event.pressed) {
        //         layer_on(_LOWER);
        //         update_tri_layer(_LOWER, _RAISE, _ADJUST);
        //     } else {
        //         layer_off(_LOWER);
        //         update_tri_layer(_LOWER, _RAISE, _ADJUST);
        //     }
        //     return false;
        //     break;
        // case RAISE:
        //     if (record->event.pressed) {
        //         layer_on(_RAISE);
        //         update_tri_layer(_LOWER, _RAISE, _ADJUST);
        //     } else {
        //         layer_off(_RAISE);
        //         update_tri_layer(_LOWER, _RAISE, _ADJUST);
        //     }
        //     return false;
        //     break;
        // case ADJUST:
        //     if (record->event.pressed) {
        //         layer_on(_ADJUST);
        //     } else {
        //         layer_off(_ADJUST);
        //     }
        //     return false;
        //     break;
        case GAME:
            if (record->event.pressed) {
                layer_on(_GAME);
            } else {
                layer_off(_GAME);
            }
            return false;
            break;
    }
    return process_record_secrets(keycode, record);
}
__attribute__((weak)) void matrix_scan_user(void) {
    achordion_task();
}
#if defined(RGBLIGHT_ENABLE)
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _RAISE:
            rgblight_setrgb(0x00, 0x00, 0xFF);
            break;
        case _LOWER:
            rgblight_setrgb(0xFF, 0x00, 0x00);
            break;
        case _GAME:
            rgblight_setrgb(0x00, 0xFF, 0x00);
            break;
        case _NAV:
            rgblight_setrgb(0x7A, 0x00, 0xFF);
            break;
        default: //  for any other layers, or the default layer
            rgblight_setrgb(0x00, 0xFF, 0xFF);
            break;
    }
    return state;
}
#endif
// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:

// Function to determine the current tapdance state
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Determine the tapdance state to return
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else
            return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // acti// A + U.on when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted)
            return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed)
            return TD_DOUBLE_HOLD;
        else
            return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed)
            return TD_TRIPLE_TAP;
        else
            return TD_TRIPLE_HOLD;
    } else
        return TD_UNKNOWN;
}
// Handle the possible states for each tapdance keycode you define:

void rguiaring_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_ARING);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_RGUI)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_ARING);
            register_code16(KC_ARING);
            break;
        case TD_TRIPLE_TAP:
            // calling into the secrets directly since I can't get
            // the keycodes working
            send_string_with_delay(secrets[2], MACRO_TIMER);
            break;
        default:
            break;
    }
}

void rguiaring_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_ARING);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_RGUI)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(KC_ARING);
            break;
        default:
            break;
    }
}
void lctld_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_D);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_D);
            register_code16(KC_D);
            break;
        default:
            break;
    }
}

void lctld_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_D);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LCTL)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(KC_D);
            break;
        default:
            break;
    }
}
void lsftf_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_F);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LSFT)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_F);
            register_code16(KC_F);
            break;
        case TD_TRIPLE_TAP:
            caps_word_toggle();
            break;
        default:
            break;
    }
}

void lsftf_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_F);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LSFT)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(KC_F);
            break;
        case TD_TRIPLE_TAP:
            break;
        default:
            break;
    }
}
void raise_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_on(_RAISE);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_RAISE)) {
                // If already set, then switch it off
                layer_off(_RAISE);
            } else {
                // If not already set, then switch the layer on
                layer_on(_RAISE);
            }
        case TD_TRIPLE_TAP:
            send_string_with_delay(secrets[1], MACRO_TIMER);
            break;
        default:
            break;
    }
}

void raise_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(_RAISE);
            break;
        default:
            break;
    }
}
void lower_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_on(_LOWER);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_LOWER)) {
                // If already set, then switch it off
                layer_off(_LOWER);
            } else {
                // If not already set, then switch the layer on
                layer_on(_LOWER);
            }
            break;
        case TD_TRIPLE_TAP:
            send_string_with_delay(secrets[0], MACRO_TIMER);
            break;
        default:
            break;
    }
}

void lower_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(_LOWER);
            break;
        default:
            break;
    }
}
// TODO: tap dance window manager move between workspaces
void wm_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            set_oneshot_mods(MOD_BIT(KC_LGUI));
            break;
        case TD_DOUBLE_TAP:
            set_oneshot_mods(MOD_BIT(KC_LGUI) | MOD_BIT(KC_LSFT));
            break;
        case TD_TRIPLE_TAP:
            caps_word_toggle();
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LGUI);
            layer_on(_LOWER);
            break;
        case TD_DOUBLE_HOLD:
            register_code(KC_LGUI);
            register_code(KC_LSFT);
            layer_on(_LOWER);
            break;
        default:
            break;
    }
}

void wm_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (td_state) {
        case TD_SINGLE_HOLD:
            clear_oneshot_mods();
            unregister_code(KC_LGUI);
            layer_off(_LOWER);
            break;
        case TD_DOUBLE_HOLD:
            clear_oneshot_mods();
            unregister_code(KC_LGUI);
            unregister_code(KC_LSFT);
            layer_off(_LOWER);
            break;
        default:
            break;
    }
}
// clang-format off
// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
    [GUI_ARING] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rguiaring_finished, rguiaring_reset),
    [LCTL_D] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lctld_finished, lctld_reset),
    [LSFT_F] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lsftf_finished, lsftf_reset),
    [LOWER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lower_finished, lower_reset),
    [RAISE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, raise_finished, raise_reset),
    [WM] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, wm_finished, wm_reset),
};
// clang-format on
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
        case KC_ARING:
        case KC_OSLASH:
        case KC_AE:
        case TD(LCTL_D):
        case TD(LSFT_F):
        case TD(GUI_ARING):
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}
