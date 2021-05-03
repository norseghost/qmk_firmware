#pragma once
#ifndef USERSPACE
#define USERSPACE

#define TAPPING_TERM 200

#include QMK_KEYBOARD_H
#include "quantum.h"
#include "wrappers.h"

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
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  SYMBOLS,
  NAV,
  ADJUST,
};
#endif
