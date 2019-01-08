#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

#define MOUSEKEY_DELAY          0
#define MOUSEKEY_INTERVAL       20
#define MOUSEKEY_MAX_SPEED      3
#define MOUSEKEY_TIME_TO_MAX    4
#define MOUSEKEY_WHEEL_DELAY    0
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

#define TAPPING_TERM 190
#define LEADER_TIMEOUT 1200

#define TAPPING_TOGGLE 1

#define IGNORE_MOD_TAP_INTERRUPT

#define TAPPING_TOGGLE 1
#define PERMISSIVE_HOLD

// When you hold a dual function key, and haven't pressed anything when you release the key,
// normally nothing happens. However, if you enable this, if you release the key without pressing another key,
// it will send the original key, even if it is outside of the tapping term.
#define RETRO_TAPPING

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 2

#endif
