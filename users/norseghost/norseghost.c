#include "norseghost.h"
#include "quantum.h"
#include "print.h"
#if (__has_include("secrets.h") && !defined(NO_SECRETS))
#    include "secrets.h"
#endif
void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    /* debug_enable = true; */
    /* debug_matrix   = true; */
    /* debug_keyboard = true; */
    /* debug_mouse    = true; */
}
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
/**/
/* __attribute__((weak)) bool is_alpha_key(keypos_t key) { */
/*   return false; */
/* } */
bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {

  // Allow same-hand holds when the other key is a non-alpha key.
  if (!is_alpha_key(other_record->event.key)) {
    return true;
  }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
    switch (tap_hold_keycode) {
        case HOME_A:
        case HOME_Z:
        case HOME_AO:
        case HOME_OE:
        case SYM_ENT:
        case SYM_BKSP:
            return 0; // Bypass Achordion for these keys.
    }

    return 400; // Otherwise use a timeout of 800 ms.
}
__attribute__((weak)) bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
    return true;
}
__attribute__((weak)) bool process_record_user(uint16_t keycode, keyrecord_t *record) { // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return true;
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
/* // Create a global instance of the tapdance state type */
/* static td_state_t td_state; */
/**/
// Declare your tapdance functions:

// Function to determine the current tapdance state
td_state_t cur_dance(tap_dance_state_t *state);

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
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

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t oslash_state = {.is_press_action = true, .state = TD_NONE};

void oslash_finished(tap_dance_state_t *state, void *user_data) {
    oslash_state.state = cur_dance(state);
    switch (oslash_state.state) {
        case TD_SINGLE_TAP:
            register_code16(KC_OSLASH);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_RGUI)); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP: // Allow nesting of 2 parens `((` within tapping term
            tap_code16(KC_OSLASH);
            register_code16(KC_OSLASH);
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

void oslash_reset(tap_dance_state_t *state, void *user_data) {
    switch (oslash_state.state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_OSLASH);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_RGUI)); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(KC_OSLASH);
            break;
        default:
            break;
    }
    oslash_state.state = TD_NONE;
}
static td_tap_t aring_state = {.is_press_action = true, .state = TD_NONE};

void aring_finished(tap_dance_state_t *state, void *user_data) {
    aring_state.state = cur_dance(state);
    switch (aring_state.state) {
        case TD_SINGLE_TAP:
            register_code16(KC_ARING);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_SYMBOLS); // For a layer-tap key, use `layer_on(_MY_LAYER)` here
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

void aring_reset(tap_dance_state_t *state, void *user_data) {
    switch (aring_state.state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_ARING);
            break;
        case TD_SINGLE_HOLD:
            layer_off(_SYMBOLS); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_TAP:
            unregister_code16(KC_ARING);
            break;
        default:
            break;
    }
    aring_state.state = TD_NONE;
}

static td_tap_t lctld_state = {.is_press_action = true, .state = TD_NONE};

void lctld_finished(tap_dance_state_t *state, void *user_data) {
    lctld_state.state = cur_dance(state);
    switch (lctld_state.state) {
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

void lctld_reset(tap_dance_state_t *state, void *user_data) {
    switch (lctld_state.state) {
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
    lctld_state.state = TD_NONE;
}

static td_tap_t lsftf_state = {.is_press_action = true, .state = TD_NONE};

void lsftf_finished(tap_dance_state_t *state, void *user_data) {
    lsftf_state.state = cur_dance(state);
    switch (lsftf_state.state) {
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

void lsftf_reset(tap_dance_state_t *state, void *user_data) {
    switch (lsftf_state.state) {
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
    lsftf_state.state = TD_NONE;
}

static td_tap_t raise_state = {.is_press_action = true, .state = TD_NONE};

void raise_finished(tap_dance_state_t *state, void *user_data) {
    raise_state.state = cur_dance(state);
    switch (raise_state.state) {
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

void raise_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (raise_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(_RAISE);
            break;
        default:
            break;
    }
    raise_state.state = TD_NONE;
}

static td_tap_t lower_state = {.is_press_action = true, .state = TD_NONE};

void lower_finished(tap_dance_state_t *state, void *user_data) {
    lower_state.state = cur_dance(state);
    switch (lower_state.state) {
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

void lower_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (lower_state.state) {
        case TD_SINGLE_HOLD:
            layer_off(_LOWER);
            break;
        default:
            break;
    }
    lower_state.state = TD_NONE;
}

static td_tap_t wm_state = {.is_press_action = true, .state = TD_NONE};

void wm_finished(tap_dance_state_t *state, void *user_data) {
    wm_state.state = cur_dance(state);
    switch (wm_state.state) {
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

void wm_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch (wm_state.state) {
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
    wm_state.state = TD_NONE;
}
// clang-format off
// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
tap_dance_action_t tap_dance_actions[] = {
    [ARING] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, aring_finished, aring_reset),
    [OSLASH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, oslash_finished, oslash_reset),
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
        case TD(ARING):
        case TD(OSLASH):
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
