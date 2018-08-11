// Based off of this section:
// https://github.com/qmk/qmk_firmware/blob/master/doc/BUILD_GUIDE.md#the-configh-file
#ifndef CONFIG_UUHAN_H
#define CONFIG_UUHAN_H

// Bring in original defaults
#include "../../config.h"

// Define mousekey settings
#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_MAX_SPEED      3
#define MOUSEKEY_TIME_TO_MAX    5
#define MOUSEKEY_WHEEL_DELAY    0

#define LEADER_TIMEOUT 800

#ifdef TAPPING_TERM
  #undef TAPPING_TERM
  #define TAPPING_TERM 210
#endif

// Disable MACRO support to shrink size
#define NO_ACTION_MACRO

// Set up tapdance functionality
//#define TAPPING_TOGGLE  1
// TAPPING_TERM is set in config.h this defaults to 200

// This makes it possible to do rolling combos (zx) with keys that convert to other keys on hold (z becomes ctrl when
// you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x. That's bad.)
#define IGNORE_MOD_TAP_INTERRUPT

#define TAPPING_TOGGLE 1
#define PERMISSIVE_HOLD

// When you hold a dual function key, and haven't pressed anything when you release the key,
// normally nothing happens. However, if you enable this, if you release the key without pressing another key,
// it will send the original key, even if it is outside of the tapping term.
#define RETRO_TAPPING

#endif
