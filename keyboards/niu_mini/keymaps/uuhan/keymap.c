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

enum layers {
  _QWERTY,
  _MOUSE_L,
  _MOUSE_R,
  _SPACEFN,
  _FNKEYS,
  _DVORAK,
  _LOWER,
  _RAISE,
  _PLOVER,
  _ADJUST,
  _BACKLIT
};

enum keycodes {
  QWERTY = SAFE_RANGE,
  SPACEFN,
  FNKEYS,
  DVORAK,
  PLOVER,
  LOWER,
  RAISE,
  BACKLIT,
  EXT_PLV
};

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
    TD_QUOT,
};

typedef struct {
    bool is_press_action;
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
  ALT_T(KC_TAB), KC_Q            , KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
  CTL_T(KC_ESC), LT(_MOUSE_L, KC_A), LT(_FNKEYS, KC_S),    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    TD(TD_SCLN), MT(MOD_RCTL, KC_ENT),
  OSM(MOD_LSFT), GUI_T(KC_Z)     , KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  TD(TD_SLSH), MT(MOD_RSFT, KC_QUOT),
  KC_LALT      , KC_LGUI         , KC_LGUI, GUI_T(KC_GRV), LT(_LOWER, KC_TAB), LT(_SPACEFN, KC_SPC), LT(_RAISE, KC_ENT),   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Mouse
 * ,---------------------------------------------------------------------------------------|
 * | Tab  |   Q  |   W  | MS_U |   R  | MOD_LGUI |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+----------+------+------+------+------+------|------|
 * | LCTL | TRNS | MS_L | MS_D | MS_L | MOD_LCTL |   H  |   J  |   K  |   L  | ACL0 | ACL2 |
 * |------+------+------+------+------+----------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  | MOD_LALT |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+----------+------+------+------+------+------+------|
 * | Ctrl | Alt  | GUI  |Brite |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_MOUSE_L] = LAYOUT_planck_mit(
  KC_TAB , KC_Q   , KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  KC_LCTL, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL  , KC_BSPC, KC_BTN1, KC_BTN3, KC_BTN2, KC_ACL0, KC_ACL1,
  KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_LALT  , KC_WH_L, KC_WH_R, KC_COMM, KC_DOT , KC_SLSH, KC_ENT ,
  KC_LCTL, KC_LGUI, KC_LALT, BACKLIT, KC_BTN2, TD(CLICK), KC_BTN3, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
),

[_MOUSE_R] = LAYOUT_planck_mit(
  KC_TAB , KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC   , KC_RGUI, KC_WH_U, KC_MS_U   , KC_WH_D, KC_P   , KC_BSPC,
  KC_ACL1, KC_ACL0, KC_BTN2, KC_BTN3, KC_BTN1, KC_LCTL   , KC_RCTL, KC_MS_L, KC_MS_D   , KC_MS_R, KC_TRNS, KC_TRNS,
  KC_LSFT, KC_Z   , KC_X   , KC_WH_L, KC_WH_R, KC_LALT   , KC_RALT, KC_M   , KC_COMM   , KC_DOT , KC_SLSH, KC_ENT ,
  KC_LCTL, KC_LGUI, KC_LALT, BACKLIT, KC_BTN3, TD(CLICK) , KC_BTN2, KC_LEFT, KC_DOWN   , KC_UP  , KC_RGHT
),

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | GUI  |Brite |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_SPACEFN] = LAYOUT_planck_mit(
  KC_BSPC, KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE, KC_TAB , KC_GRV , KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_TRNS,
  KC_ENT , KC_LPRN, KC_LEFT, KC_DOWN, KC_RGHT, KC_MINS, KC_EQL , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, TD(TD_QUOT),
  KC_LSFT, KC_RPRN, KC_LCBR, KC_RCBR, KC_BSLS, KC_UNDS, KC_PLUS, KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_ENT,
  KC_LCTL, KC_LGUI, KC_LALT, BACKLIT, LOWER  ,     _______     , RAISE  , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT
),

[_FNKEYS] = LAYOUT_planck_mit(
  KC_BSLS, KC_EQL , KC_MINS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , _______, _______, _______, _______, _______,
  KC_ENT , KC_LPRN, _______, KC_BSPC, KC_DEL , _______, _______, _______, _______, _______, _______, _______,
  KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
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
  KC_TAB,  KC_QUOT, KC_COMM, KC_MS_U,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
  KC_ESC,  KC_A,    KC_MS_L,    KC_MS_D,    KC_MS_R,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
  KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT ,
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
  ALT_T(KC_ESC) , KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7   , KC_8   ,  KC_9   , KC_0   ,  KC_DEL,
  GUI_T(KC_BSPC), KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, KC_MINS, KC_EQL ,  KC_LBRC, KC_RBRC,  KC_GRV,
  F(0)          , KC_MINS, KC_EQL , KC_BSLS, KC_GRV,  KC_BSPC, _______, _______, _______,  _______, KC_BSLS, _______,
  _______, _______, _______, _______, _______,     _______,     RAISE, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
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
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_TILD,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_LT  , KC_GT  , KC_PIPE, KC_DQUO,
  _______, _______, _______, _______, _______,     _______,      LOWER, KC_HOME, KC_PGDN, KC_PGUP,  KC_END
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
  _______, RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______, _______, KC_DEL ,
  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  SPACEFN, DVORAK,  PLOVER,  _______,
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
  switch (xtap_scln_state.state) {
    case SINGLE_TAP:
        register_code(KC_SCLN);
        break;
    case SINGLE_HOLD:
        layer_on(_MOUSE_R);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_SCLN);
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
        register_code(KC_RGUI);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_SLSH);
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
        del_weak_mods(MOD_LSFT);
        unregister_code(KC_SLSH);
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

qk_tap_dance_action_t tap_dance_actions[] = {
    [CLICK]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [TD_SCLN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,scln_finished,scln_reset),
    [TD_SLSH] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,slsh_finished,slsh_reset),
    [TD_QUOT] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
};

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_MODS_TAP_TOGGLE(MOD_LSFT),
};
