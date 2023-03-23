#pragma once

// Right-hand home row mods
// layer taps
#define SYM_BKSP LT(_SYMBOLS, KC_BSPC)
#define SYM_ENT LT(_SYMBOLS, KC_ENTER)
#define LWR_SPC LT(_LOWER, KC_SPACE)
#define RSE_SPC LT(_RAISE, KC_SPACE)
#define SYM_SPC LT(_SYMBOLS, KC_SPACE)
#define SYM_A LT(_SYMBOLS, KC_A)
// requires OS input language set to US international
#define KC_ARING RALT(KC_W)
#define KC_OSLASH RALT(KC_L)
#define KC_AE RALT(KC_Z)
// home row mods
#define HOME_Z LGUI_T(KC_Z)
#define HOME_A LT(_SYMBOLS, KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)
#define HOME_G LT(_NAV, KC_G)

#define HOME_H KC_H
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L RALT_T(KC_L)
#define HOME_AO TD(ARING)
#define HOME_OE TD(OSLASH)
#ifndef MACRO_TIMER
#    define MACRO_TIMER 5
#endif
#ifndef USERSPACE

#    define USERSPACE

#    define TAPPING_TERM 150
#    define TAPPING_TERM_PER_KEY
#    define IGNORE_MOD_TAP_INTERRUPT
#    define TAPPING_FORCE_HOLD
#    define PERMISSIVE_HOLD
#    define SPLIT_TRANSPORT_MIRROR

#    include QMK_KEYBOARD_H
#    include "quantum.h"
#    include "wrappers.h"

/* Define layer names */
enum userspace_layers {
    _QWERTY = 0,
    _NAV,
    _SYMBOLS,
    _LOWER,
    _RAISE,
    _GAME,
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    SYMBOLS,
    NAV,
    ADJUST,
    MY_HASH,
    MY_EXCL,
    GAME,
    SECRET_1,
    SECRET_2,
    SECRET_3,
    SECRET_4,
    SECRET_5,
};
#endif
// Tap Dance keycodes
enum td_keycodes {
    ARING,  // right gui when held, "å" when tapped
    OSLASH, // right gui when held, "ø" when tapped
    LCTL_D, // left control when held, "d" when tapped
    LSFT_F, // left shift when held, "f" when tapped
    LOWER,
    RAISE,
    WM,
};

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool       is_press_action;
    td_state_t state;
} td_tap_t;
