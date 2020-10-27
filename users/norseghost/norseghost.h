#pragma once
#ifndef USERSPACE
#define USERSPACE

#include "quantum.h"
#include "wrappers.h"
#include QMK_KEYBOARD_H

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
