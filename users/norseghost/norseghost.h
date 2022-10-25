#pragma once

// Right-hand home row mods
// layer taps
#define SYM_BKSP LT(_SYMBOLS, KC_BSPC)
#define SYM_ENT LT(_SYMBOLS, KC_ENTER)
#define LWR_SPC LT(_LOWER, KC_SPACE)
#define RSE_SPC LT(_RAISE, KC_SPACE)
#define SYM_SPC LT(_SYMBOLS, KC_SPACE)
#ifndef USERSPACE
#    define USERSPACE

#    define TAPPING_TERM 150
#    define IGNORE_MOD_TAP_INTERRUPT
#    define TAPPING_FORCE_HOLD

#    include QMK_KEYBOARD_H
#    include "quantum.h"
#    include "wrappers.h"

/* Define layer names */
enum userspace_layers {
    _QWERTY  = 0,
    _NUMLOCK = 0,
    _COLEMAK,
    _DVORAK,
    _WORKMAN,
    _NORMAN,
    _MALTRON,
    _EUCALYN,
    _CARPLAX,
    _NAV, /* layer 8 */
    _SYMBOLS,
    _MACROS,
    _MEDIA,
    _LOWER,
    _RAISE,
    _ADJUST,
    _GAME,
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
    SYMBOLS,
    NAV,
    ADJUST,
    MY_HASH,
    MY_EXCL,
    GAME,
};
#endif
