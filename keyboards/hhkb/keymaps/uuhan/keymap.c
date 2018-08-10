#include QMK_KEYBOARD_H

#include "quantum.h"
#include "action.h"
#include "mousekey.h"
#include "process_keycode/process_tap_dance.h"

extern keymap_config_t keymap_config;

enum layers {
    _BASE = 0,
    _HHKB,
    _MOUSE_L,
    _MOUSE_R,
    _MIRROR,
    _SPACEFN,
    _OTHER,
};

enum keycodes {
    BASE,
    HHKB,
    MOUSE_L,
    MOUSE_R,
    MIRROR,
    SPACEFN,
    OTHER,
};

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5,
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
};

enum {
    CLICK = 0,
    SPACE,
    QUOTE,
};

typedef struct {
    bool is_press_action;
    int state;
} xtap;

// Required for leader function. Measured in ms
// #define LEADER_TIMEOUT 300

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT( //  default layer
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_LEAD,
        GUI_T(KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
        CTL_T(KC_ESC), LT(_MOUSE_L, KC_A), LT(_OTHER, KC_S), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, LT(_MOUSE_R, KC_SCLN), TD(QUOTE), MT(KC_RCTL, KC_ENT),
        OSM(MOD_LSFT), GUI_T(KC_Z), KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, MT(KC_RGUI, KC_SLSH), MT(KC_RSFT, KC_ESC), TT(HHKB),
                            KC_LALT, LT(_MIRROR, KC_GRV), TD(SPACE), TT(_MIRROR), KC_RALT),

    [_HHKB] = LAYOUT(
        KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_ENT,
        KC_LSFT, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_RSFT, KC_TRNS,
                                KC_LALT, KC_LGUI, KC_LGUI, KC_RGUI, KC_RALT),

    [_MOUSE_L] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TAB , KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI, KC_ESC , KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_TRNS, KC_TRNS, KC_BSPC,
        KC_ACL0, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL, KC_BSPC, KC_BTN1, KC_BTN3, KC_BTN2, KC_BTN4, KC_BTN5, KC_ENT,
        KC_LSFT, KC_ACL0, KC_TRNS, KC_BTN3, KC_BTN2, KC_LALT, KC_WH_L, KC_WH_R, KC_ACL0, KC_ACL1, KC_ACL2, KC_TRNS, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_RALT, KC_RGUI),

    [_MOUSE_R] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_ESC , KC_RGUI, KC_WH_D, KC_MS_U, KC_WH_U, KC_ACL0, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_BTN5, KC_BTN4, KC_BTN2, KC_BTN3, KC_BTN1, KC_DEL , KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_ACL0, KC_ENT,
        KC_LSFT, KC_ACL2, KC_ACL1, KC_ACL0, KC_WH_L, KC_WH_R, KC_RALT, KC_BTN2, KC_BTN3, KC_TRNS, KC_TRNS, KC_RSFT, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_TRNS, KC_TRNS),

    [_MIRROR] = LAYOUT(
        KC_GRV, KC_BSLS, KC_EQL, KC_MINS, KC_0, KC_9, KC_8, KC_7, KC_6, KC_5, KC_4, KC_3, KC_2, KC_1, KC_ESC,
        KC_BSPC, KC_P, KC_O, KC_I, KC_U, KC_Y, KC_T, KC_R, KC_E, KC_W, KC_Q, KC_LBRC, KC_RBRC, KC_TAB,
        RCTL_T(KC_ENT), KC_SCLN, KC_L, KC_K, KC_J, KC_H, KC_G, KC_F, KC_D, KC_S, KC_A, KC_QUOT, CTL_T(KC_ESC),
        KC_RSFT, KC_SLSH, KC_DOT, KC_COMM, KC_M, KC_N, KC_B, KC_V, KC_C, KC_X, KC_Z, KC_LSFT, KC_TRNS,
                                KC_RALT, KC_TRNS, KC_SPC, KC_TRNS, KC_LALT),

    [_SPACEFN] = LAYOUT(
        KC_BSLS, KC_EQL, KC_MINS, KC_0   , KC_9   , KC_8    , KC_7   , KC_P7  , KC_P8, KC_P9  , KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, RESET,
        GUI_T(KC_BSPC) , KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE , KC_BSLS, KC_BSPC, KC_P4, KC_P5  , KC_P6  , KC_PENT, KC_PAST, KC_PSLS, KC_TAB,
        CTL_T(KC_ENT)  , KC_LPRN, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGUP, KC_HOME, KC_P1, KC_P2  , KC_P3  , KC_MINS, KC_EQL , KC_ENT,
        ACTION_MODS_TAP_TOGGLE(MOD_LSFT)       , KC_RPRN, KC_LCBR, KC_RCBR, KC_PGDN , KC_END , KC_DEL , KC_P0, KC_PEQL, KC_PDOT, KC_TRNS, KC_TRNS, KC_TRNS,
                                KC_RALT, KC_RGUI, KC_TRNS, KC_LGUI, KC_LALT),

    [_OTHER] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_TRNS,
        KC_TAB , KC_F7, KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_F8, KC_TRNS, KC_BSPC, KC_DEL , KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_MUTE, KC_TRNS, KC_MPLY, KC_ENT,
        KC_LSFT, KC_F9, KC_F10, KC_F11  , KC_F12 , KC_F13 , KC_F14 , KC_F15, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT, KC_TRNS,
                                KC_LGUI, KC_LALT, KC_ENT, KC_RGUI, KC_RALT)};

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
static xtap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void click_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:
        mousekey_on(KC_BTN1);
        break;
    case SINGLE_HOLD:
        mousekey_on(KC_ACL0);
        mousekey_send();
        break;
    case DOUBLE_TAP:
        mousekey_on(KC_BTN2);
        break;
    case DOUBLE_HOLD:
        mousekey_on(KC_BTN1);
        mousekey_send();
        break;
    case DOUBLE_SINGLE_TAP:
        mousekey_on(KC_BTN1);
        mousekey_send();
        mousekey_off(KC_BTN1);
        mousekey_send();
  }
}

void click_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
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
        mousekey_send();
        mousekey_off(KC_BTN2);
        mousekey_send();
        break;
    case DOUBLE_HOLD:
        mousekey_off(KC_BTN1);
        mousekey_send();
        break;
    case DOUBLE_SINGLE_TAP:
        mousekey_on(KC_BTN1);
        mousekey_send();
        mousekey_off(KC_BTN1);
        mousekey_send();
  }
  xtap_state.state = 0;
}

void space_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:
        register_code(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_on(_SPACEFN);
        break;
    case DOUBLE_TAP:
        register_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        register_code(KC_LGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        register_code(KC_SPC);
  }
}

void space_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_off(_SPACEFN);
        break;
    case DOUBLE_TAP:
        unregister_code(KC_ENT);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_LGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SPC);
  }
  xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [CLICK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,space_finished,space_reset),
    [QUOTE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_DQUO),
};
