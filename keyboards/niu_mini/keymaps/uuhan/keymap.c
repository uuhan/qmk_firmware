/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "action.h"
#include "mousekey.h"
#include "process_keycode/process_tap_dance.h"

extern keymap_config_t keymap_config;

void send_keycode(uint8_t kc){
    register_code(kc);
    unregister_code(kc);
}

enum layers {
  _QWERTY,
  _FNS,
  _MACRO,
  _MOUSE_L,
  _MOUSE_R,
  _SPACEFN,
  _MIRROR,
  _FNKEYS,
  _DVORAK,
  _LOWER,
  _RAISE,
  _PLOVER,
  _ADJUST,
  _BACKLIT
};

enum keycodes {
    // TH
    TH_COMM = SAFE_RANGE,
    TH_DOT,
    //  TH

    QWERTY,
    SPACEFN,
    FNKEYS,
    DVORAK,
    MOUSE,
    PLOVER,
    LOWER,
    RAISE,
    BACKLIT,
    EXT_PLV,

    // dynamic macros
    DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"

// tap-hold settings
#define LONGPRESS_DELAY 180
const int TH_EVENTS_COUNT = TH_DOT - TH_COMM;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5, //send two single taps
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
};

//Tap dance enums
enum {
    CLICK = 0,
    TD_SCLN,
    TD_SLSH,
    TD_PIPE,
    TD_QUOT,
    TD_COMM,
    TD_DOT,
    TD_QUOT_SHIFT,
    TD_SPACE,
    TD_LOWER,
    TD_RAISE,
};

typedef struct {
    bool is_press_action;
    bool is_keeping;
    int state;
} xtap;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | GUI  |Brite |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_mit(
  GUI_T(KC_TAB)   , KC_Q              , KC_W             , KC_E       , KC_R, KC_T, KC_Y, KC_U, KC_I   , KC_O  , KC_P       , LT(_FNS, KC_BSPC)    ,
  LCTL_T(KC_ESC) , LT(_MOUSE_L, KC_A), LT(_FNKEYS, KC_S), KC_D       , KC_F, KC_G, KC_H, KC_J, KC_K   , KC_L  , TD(TD_SCLN), MT(MOD_RCTL , KC_ENT),
  OSM(MOD_LSFT)  , GUI_T(KC_Z)       , CTL_T(KC_X)      , ALT_T(KC_C), KC_V, KC_B, KC_N, KC_M, RALT_T(KC_COMM), RCTL_T(KC_DOT), TD(TD_SLSH), TD(TD_QUOT_SHIFT)    ,
  KC_LALT        , MO(_MACRO)        , SH_TT            , GUI_T(KC_GRV), TD(TD_LOWER), TD(TD_SPACE), TD(TD_RAISE), KC_MINS, KC_EQL, DYN_MACRO_PLAY2, DYN_MACRO_PLAY1
),

[_MACRO] = LAYOUT_planck_mit(
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    DYN_REC_START2, DYN_REC_START1,
    RGB_MOD, RGB_TOG, DYN_REC_STOP, DYN_REC_START2, DYN_REC_START1, _______, _______, DYN_MACRO_PLAY1, DYN_REC_START2, _______, _______, DYN_REC_STOP,
    _______, BL_TOGG, BL_STEP, RGB_HUI, RGB_HUD, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______
),

[_FNS] = LAYOUT_planck_mit(
    _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5 , KC_F11 , KC_F12  , KC_F13 , KC_F14 , KC_F15, _______,
    _______, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10, KC_F16 , KC_F17  , KC_F18 , KC_F19 , KC_F20, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______
),

[_MOUSE_L] = LAYOUT_planck_mit(
  TO(_ADJUST), KC_Q   , KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  KC_LCTL    , KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL  , KC_BSPC, KC_BTN1, KC_BTN3, KC_BTN2, KC_ACL0, KC_ACL1,
  KC_LSFT    , KC_Z   , KC_WH_L, KC_BTN3, KC_WH_R, KC_LALT  , _______, _______, KC_WH_L, KC_WH_R, KC_SLSH, KC_ENT ,
  KC_LCTL    , KC_LGUI, KC_LALT, _______, KC_BTN2, TD(CLICK), KC_BTN3, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
),

[_MOUSE_R] = LAYOUT_planck_mit(
  KC_TAB , KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC   , KC_RGUI, KC_WH_U, KC_MS_U   , KC_WH_D, KC_P   , KC_BSPC,
  KC_ACL1, KC_ACL0, KC_BTN2, KC_BTN3, KC_BTN1, KC_LCTL   , KC_RCTL, KC_MS_L, KC_MS_D   , KC_MS_R, KC_TRNS, KC_TRNS,
  KC_LSFT, KC_Z   , KC_WH_L, KC_WH_R, _______, KC_LALT   , KC_RALT, KC_WH_L, KC_BTN3   , KC_WH_R, KC_SLSH, KC_ENT ,
  KC_LCTL, KC_LGUI, KC_LALT, _______, KC_BTN3, TD(CLICK) , KC_BTN2, KC_LEFT, KC_DOWN   , KC_UP  , KC_RGHT
),

[_SPACEFN] = LAYOUT_planck_mit(
  GUI_T(KC_ESC) , KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE, KC_TAB , KC_GRV , KC_UNDS, KC_PLUS , KC_TILD , KC_PIPE , _______ ,
  CTL_T(KC_ENT) , KC_LPRN, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSLS, KC_LEFT, KC_DOWN, KC_UP   , KC_RGHT , KC_GRV  , _______,
  KC_LSFT       , KC_RPRN, KC_LCBR, KC_RCBR, KC_BSLS, KC_UNDS, KC_PLUS, KC_HOME, KC_PGUP , KC_PGDN , KC_END  , KC_TRNS,
  KC_LALT       , _______, _______, _______, LOWER  , _______, RAISE  , KC_LEFT, KC_DOWN , KC_UP   , KC_RGHT
),

[_MIRROR] = LAYOUT_planck_mit(
  KC_BSPC, KC_P   , KC_O,    KC_I,    KC_U,    KC_Y,    KC_T,    KC_R,    KC_E,    KC_W,    KC_Q,  KC_TAB,
  RCTL_T(KC_ENT) , KC_SCLN, KC_L,    KC_K,    KC_J,    KC_H,    KC_G,    KC_F,    KC_D,    KC_S,  KC_A, KC_LCTL,
  KC_TRNS, KC_SLSH, KC_COMM, KC_DOT,  KC_M,    KC_N,    KC_B,    KC_V,    KC_C, KC_X,  KC_Z, _______,
  KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, RAISE, KC_SPC, LOWER,   KC_LGUI, _______, _______,  KC_LALT
),

[_FNKEYS] = LAYOUT_planck_mit(
  KC_BSLS       , KC_F5  , _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_UNDS, KC_PLUS, KC_ENT,  KC_PIPE, KC_TILD,
  CTL_T(KC_ENT) , KC_BSPC, _______, KC_DEL , KC_ENT , KC_PIPE, KC_BSLS, KC_MINS, KC_EQL , KC_GRV,  KC_QUOT, KC_DQUO,
  KC_LSFT, KC_F9, KC_F10 , KC_F11 , KC_F12 , KC_F13 , KC_F14 , KC_F15 , KC_LT, KC_GT, KC_SLSH, KC_RSFT,
  _______, _______, _______, _______, KC_ENT ,      KC_ENT     , KC_TAB , _______, _______, _______, _______
),

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   "  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | GUI  |Brite |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = LAYOUT_planck_mit(
  KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
  KC_ESC,  KC_A,    KC_O   , KC_E  ,  KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
  KC_LSFT, KC_SCLN, KC_Q   , KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT ,
  KC_LCTL, KC_LGUI, KC_LALT, BACKLIT, LOWER,       KC_SPC,       RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |   4  |   5  |   6  |      |      |      |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   7  |   8  |   9  |      |      |      |ISO # |ISO / |Pg Up |Pg Dn |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |   0  |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_mit(
  ALT_T(KC_BSPC), KC_1, KC_2, KC_3   , KC_4  , KC_5   , KC_6   , KC_7   , KC_8   ,  KC_9   , KC_0   ,  _______,
  CTL_T(KC_ENT) , KC_6, KC_7, KC_8   , KC_9  , KC_0   , _______, KC_MINS, KC_EQL ,  KC_LBRC, KC_RBRC,  KC_GRV,
  _______       , _______, _______, _______, _______, _______ , _______, _______, _______,  _______, KC_BSLS, _______,
  _______       , _______, _______, _______, _______,     _______,     RAISE, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
),

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | |      | End  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_mit(
  _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL , KC_BSLS, KC_GRV , KC_TRNS,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_LT  , KC_GT  , KC_PIPE, KC_DQUO,
  _______, _______, _______, _______, LOWER ,     _______,      _______, KC_HOME, KC_PGDN, KC_PGUP,  KC_END
),

/* Plover layer (http://opensteno.org)
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */

[_PLOVER] = LAYOUT_planck_mit(
  KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1   ,
  XXXXXXX, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
  XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  EXT_PLV, XXXXXXX, XXXXXXX, KC_C,    KC_V,        XXXXXXX,      KC_N,    KC_M,    XXXXXXX, XXXXXXX, XXXXXXX
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_mit(
  SH_TT  , RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______, _______, SH_TT,
  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY , DVORAK  , MOUSE , PLOVER,  _______,
  _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______,     _______,      _______, _______, _______, _______, _______
),

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  TOG |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |  MOD |  HUI |  HUD |
 * `-----------------------------------------------------------------------------------'
 */
[_BACKLIT] = LAYOUT_planck_mit(
  _______, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, RGB_M_SN, RGB_M_K, RGB_M_X, RGB_M_G, _______, _______, _______,
  RGB_TOG, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAI, RGB_VAD,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SAI, RGB_SAD,
  _______, _______, _______, _______, _______,     _______,      _______, RGB_MOD, RGB_RMOD, RGB_HUI, RGB_HUD
)


};

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

typedef struct {
    bool is_pressed;
    uint16_t timer;
    uint16_t kc_tap;
    uint16_t kc_hold;
} tap_hold_t;

static tap_hold_t th_events[] = {
    { .is_pressed = false, .timer = 0, .kc_tap = KC_COMM,    .kc_hold = KC_MINS  }, // TH_COMM
    { .is_pressed = false, .timer = 0, .kc_tap = KC_DOT ,    .kc_hold = KC_EQL   }, // TH_EQL
};

void taphold_tapped(uint8_t index, bool pressed) {
    if (index > TH_EVENTS_COUNT) { return; }

    tap_hold_t *th_event = &th_events[index];

    if (pressed) {
        th_event->timer = timer_read();
        th_event->is_pressed = true;
    } else if (th_event->is_pressed) {
        register_code(th_event->kc_tap);
        unregister_code(th_event->kc_tap);
        th_event->is_pressed = false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t macro_kc = (keycode == MO(_MACRO) ? DYN_REC_STOP : keycode);
    if (!process_record_dynamic_macro(macro_kc, record)) {
        return false;
    }
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                print("mode just switched to qwerty and this is a huge string\n");
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case SPACEFN:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_SPACEFN);
            }
            return false;
            break;
        case DVORAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_DVORAK);
            }
            return false;
            break;
        case MOUSE:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_MOUSE_L);
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
        case BACKLIT:
            if (record->event.pressed) {
                layer_on(_BACKLIT);
                // update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_BACKLIT);
                // update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case PLOVER:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
#endif
                layer_off(_RAISE);
                layer_off(_LOWER);
                layer_off(_ADJUST);
                layer_on(_PLOVER);
                if (!eeconfig_is_enabled()) {
                    eeconfig_init();
                }
                keymap_config.raw = eeconfig_read_keymap();
                keymap_config.nkro = 1;
                eeconfig_update_keymap(keymap_config.raw);
            }
            return false;
            break;
        case EXT_PLV:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                PLAY_SONG(plover_gb_song);
#endif
                layer_off(_PLOVER);
            }
            return false;
            break;
        case TH_COMM ... TH_DOT:
            taphold_tapped(keycode - TH_COMM, record->event.pressed);
            return false;
    }
    return true;
}

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static xtap xtap_cick_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_scln_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_slsh_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_space_state = {
  .is_press_action = true,
  .is_keeping = false,
  .state = 0
};

static xtap xtap_quot_state = {
  .is_press_action = true,
  .is_keeping = false,
  .state = 0
};

static xtap xtap_lower_state = {
  .is_press_action = true,
  .is_keeping = false,
  .state = 0
};

static xtap xtap_raise_state = {
  .is_press_action = true,
  .is_keeping = false,
  .state = 0
};

void click_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_cick_state.state = cur_dance(state);
  switch (xtap_cick_state.state) {
    case SINGLE_TAP:
        mousekey_on(KC_BTN1);
        break;
    case SINGLE_HOLD:
        mousekey_on(KC_ACL0);
        mousekey_send();
        break;
    case DOUBLE_TAP:
        mousekey_on(KC_BTN2);
        mousekey_send();
        break;
    case DOUBLE_HOLD:
        mousekey_on(KC_BTN1);
        mousekey_send();
        break;
    case DOUBLE_SINGLE_TAP:
        mousekey_on(KC_BTN1);
        mousekey_send();
  }
}

void click_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_cick_state.state) {
    case SINGLE_TAP:
        mousekey_send();
        mousekey_off(KC_BTN1);
        mousekey_send();
        break;
    case SINGLE_HOLD:
        mousekey_off(KC_ACL0);
        mousekey_send();
        break;
    case DOUBLE_TAP:
        mousekey_off(KC_BTN2);
        mousekey_send();
        break;
    case DOUBLE_HOLD:
        mousekey_off(KC_BTN1);
        mousekey_send();
        break;
    case DOUBLE_SINGLE_TAP:
        mousekey_send();
        mousekey_off(KC_BTN1);
        mousekey_send();
  }
  xtap_cick_state.state = 0;
}

void scln_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_scln_state.state = cur_dance(state);
  uint8_t rctl_mask = get_mods() & (MOD_BIT(KC_RCTL));
  uint8_t lsft_mask = get_mods() & (MOD_BIT(KC_LSFT));
  switch (xtap_scln_state.state) {
    case SINGLE_TAP:
        register_code(KC_SCLN);
        break;
    case SINGLE_HOLD:
        if (rctl_mask) {
            del_mods(MOD_BIT(KC_RCTL));
            layer_on(_LOWER);
            return;
        } else {
            layer_on(_MOUSE_R);
            return;
        }
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_SCLN);
        if (lsft_mask) {
            unregister_code(KC_SCLN);
            register_code(KC_SCLN);
        } else {}
        break;
    case DOUBLE_HOLD:
        register_code(KC_RALT);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_SCLN);
        unregister_code(KC_SCLN);
        register_code(KC_SCLN);
  }
}

void scln_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_scln_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_SCLN);
        break;
    case SINGLE_HOLD:
        layer_off(_LOWER);
        layer_off(_MOUSE_R);
        break;
    case DOUBLE_TAP:
        del_weak_mods(MOD_LSFT);
        unregister_code(KC_SCLN);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_RALT);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SCLN);
  }
  xtap_scln_state.state = 0;
}

void slsh_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_slsh_state.state = cur_dance(state);
  switch (xtap_slsh_state.state) {
    case SINGLE_TAP:
        register_code(KC_SLSH);
        break;
    case SINGLE_HOLD:
        if (!state->interrupted) {
            register_code(KC_RGUI);
        }
        break;
    case DOUBLE_TAP:
        break;
    case DOUBLE_HOLD:
        register_code(KC_RSFT);
        register_code(KC_RGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_SLSH);
        unregister_code(KC_SLSH);
        register_code(KC_SLSH);
  }
}

void slsh_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_slsh_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_SLSH);
        break;
    case SINGLE_HOLD:
        unregister_code(KC_RGUI);
        break;
    case DOUBLE_TAP:
        SEND_STRING("//");
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_RSFT);
        unregister_code(KC_RGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SLSH);
  }
  xtap_slsh_state.state = 0;
}

void space_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_space_state.state = cur_dance(state);
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        send_keycode(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_on(_SPACEFN);
        break;
    case DOUBLE_TAP:
        if (!state->interrupted) {
            register_code(KC_LCTL);
            register_code(KC_SPC);
            unregister_code(KC_SPC);
            unregister_code(KC_LCTL);
        } else {
            send_keycode(KC_SPC);
            send_keycode(KC_SPC);
        }
        break;
    case DOUBLE_HOLD:
        register_code(KC_SPC);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        register_code(KC_SPC);
  }
}

void space_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        break;
    case SINGLE_HOLD:
        layer_off(_SPACEFN);
        break;
    case DOUBLE_TAP:
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_SPC);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SPC);
  }
  xtap_space_state.state = 0;
}

void quot_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_quot_state.state = cur_dance(state);
  switch (xtap_quot_state.state) {
    case SINGLE_TAP:
        // xtap_quot_state.timestamp = state->timer;
        register_code(KC_QUOT);
        break;
    case SINGLE_HOLD:
        register_code(KC_RSFT);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_QUOT);
        break;
    case DOUBLE_HOLD:
        register_code(KC_RSFT);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_RSFT);
        unregister_code(KC_RSFT);
        register_code(KC_RSFT);
  }
}

void quot_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_quot_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_QUOT);
        break;
    case SINGLE_HOLD:
        unregister_code(KC_RSFT);
        break;
    case DOUBLE_TAP:
        del_weak_mods(MOD_LSFT);
        unregister_code(KC_QUOT);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_RSFT);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_RSFT);
  }
  xtap_quot_state.state = 0;
}

void lower_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_lower_state.state = cur_dance(state);
  uint8_t lctl_mask = get_mods() & (MOD_BIT(KC_LCTL));
  uint8_t space_mask = get_mods() & (MOD_BIT(KC_LSFT));
  switch (xtap_lower_state.state) {
    case SINGLE_TAP:
        if (state->interrupted) {
            layer_on(_LOWER);
        } else {
            register_code(KC_TAB);
            unregister_code(KC_TAB);
        }
        break;
    case SINGLE_HOLD:
        if (lctl_mask) {
            layer_on(_MIRROR);
            return;
        } if (space_mask) {
            del_mods(MOD_BIT(KC_LSFT));
            layer_on(_MIRROR);
            return;
        } else {
            layer_on(_LOWER);
            return;
        }
        break;
    case DOUBLE_TAP:
        register_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        layer_on(_RAISE);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_TAB);
        unregister_code(KC_TAB);
        register_code(KC_TAB);
  }
}

void lower_reset (qk_tap_dance_state_t *state, void *user_data) {
  uint8_t rgui_mask = get_mods() & (MOD_BIT(KC_RGUI));
  if (rgui_mask) {
      del_mods(KC_RGUI);
  }
  switch (xtap_lower_state.state) {
    case SINGLE_TAP:
        layer_off(_LOWER);
        break;
    case SINGLE_HOLD:
        layer_off(_LOWER);
        layer_off(_MIRROR);
        break;
    case DOUBLE_TAP:
        unregister_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        layer_off(_RAISE);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_TAB);
  }
  xtap_lower_state.state = 0;
}

void raise_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_raise_state.state = cur_dance(state);
  uint8_t rctl_mask = get_mods() & (MOD_BIT(KC_RCTL));
  uint8_t rsft_mask = get_mods() & (MOD_BIT(KC_RSFT));
  switch (xtap_raise_state.state) {
    case SINGLE_TAP:
        if (state->interrupted) {
            layer_on(_RAISE);
        } else {
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }
        break;
    case SINGLE_HOLD:
        if (rctl_mask) {
            layer_on(_MIRROR);
            return;
        } if (rsft_mask) {
            del_mods(MOD_BIT(KC_RSFT));
            layer_on(_MIRROR);
            return;
        } else {
            layer_on(_RAISE);
            return;
        }
        break;
    case DOUBLE_TAP:
        register_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        register_code(KC_ENT);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_ENT);
        unregister_code(KC_ENT);
        register_code(KC_ENT);
  }
}

void raise_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_raise_state.state) {
    case SINGLE_TAP:
        layer_off(_RAISE);
        break;
    case SINGLE_HOLD:
        layer_off(_MIRROR);
        layer_off(_RAISE);
        break;
    case DOUBLE_TAP:
        unregister_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_ENT);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_ENT);
  }
  xtap_raise_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [CLICK]         = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [TD_SCLN]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL,scln_finished,scln_reset),
    [TD_SLSH]       = ACTION_TAP_DANCE_FN_ADVANCED(NULL,slsh_finished,slsh_reset),
    [TD_PIPE]       = ACTION_TAP_DANCE_DOUBLE(KC_PIPE, KC_BSLS),
    [TD_QUOT]       = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
    [TD_SPACE]      = ACTION_TAP_DANCE_FN_ADVANCED(NULL,space_finished,space_reset),
    [TD_LOWER]      = ACTION_TAP_DANCE_FN_ADVANCED(NULL,lower_finished,lower_reset),
    [TD_RAISE]      = ACTION_TAP_DANCE_FN_ADVANCED(NULL,raise_finished,raise_reset),
    [TD_QUOT_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,quot_finished,quot_reset),
    [TD_COMM]       = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_MINS),
    [TD_DOT]        = ACTION_TAP_DANCE_DOUBLE(KC_DOT, KC_EQL),
};

/* const uint16_t PROGMEM fn_actions[] = { */
/*     [0] = ACTION_MODS_TAP_TOGGLE(MOD_LSFT), */
/* }; */

const keypos_t hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
  {{11, 0}, {10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}},
  {{11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
  {{11, 2}, {10, 2}, {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
  {{11, 3}, {10, 3}, {9, 3}, {8, 3}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}},
};

void matrix_scan_user(void) {
    for (uint8_t index = 0 ; index <= TH_EVENTS_COUNT ; ++index ) {
        tap_hold_t *th_event = &th_events[index];
        if ( th_event->is_pressed && timer_elapsed(th_event->timer) > LONGPRESS_DELAY) {
            register_code(th_event->kc_hold);
            unregister_code(th_event->kc_hold);
            th_event->is_pressed = false;
        }
    }
}
