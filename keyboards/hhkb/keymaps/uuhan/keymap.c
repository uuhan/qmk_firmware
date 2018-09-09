#include QMK_KEYBOARD_H

#include "quantum.h"
#include "action.h"
#include "mousekey.h"
#include "process_keycode/process_tap_dance.h"

extern keymap_config_t keymap_config;

enum layers {
    BASE = 0,
    HHKB,
    MOUSE_L,
    MOUSE_R,
    MIRROR,
    SPACEFN,
    FNKEYS,
};

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
    DOUBLE_HOLD = 4,
    DOUBLE_SINGLE_TAP = 5,
};

enum {
    CLICK = 0,
    SPACE,
    QUOTE,
    SCLN,
};

enum my_keycods {
    DYNAMIC_MACRO_RANGE,
};

#include "dynamic_macro.h"

typedef struct {
    bool is_press_action;
    bool is_keeping;
    // uint16_t timestamp;
    int state;
} xtap;

// Required for leader function. Measured in ms
// #define LEADER_TIMEOUT 300

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [BASE] = LAYOUT( //  default layer
        ALT_T(KC_ESC), KC_1 , KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, DYN_MACRO_PLAY1,
        GUI_T(KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
        CTL_T(KC_ESC), LT(MOUSE_L, KC_A), LT(FNKEYS, KC_S), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, TD(SCLN), TD(QUOTE), MT(MOD_RCTL, KC_ENT),
        OSM(MOD_LSFT), GUI_T(KC_Z), CTL_T(KC_X), ALT_T(KC_C), KC_V, KC_B, KC_N, KC_M, RALT_T(KC_COMM), RCTL_T(KC_DOT), RGUI_T(KC_SLSH), MT(MOD_RSFT, KC_ESC), MO(HHKB),
                        KC_LALT, LT(MIRROR, KC_GRV), TD(SPACE), TT(MIRROR), KC_RALT),

    [HHKB] = LAYOUT(
        KC_GRV , KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_ENT,
        KC_LSFT, DYN_REC_START1, DYN_REC_STOP, DYN_MACRO_PLAY1, KC_TRNS, KC_TRNS, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_RSFT, KC_TRNS,
                                KC_LALT, KC_LGUI, KC_LGUI, KC_RGUI, KC_RALT),

    [MOUSE_L] = LAYOUT(
        KC_TRNS, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD,
        KC_TAB , KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI, KC_ESC , KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_TRNS, KC_TRNS, KC_BSPC,
        KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL, KC_BSPC, KC_BTN1, KC_BTN3, KC_BTN2, KC_BTN4, KC_BTN5, KC_ACL0,
        KC_LSFT, KC_ACL0, KC_WH_L, KC_BTN3, KC_WH_R, KC_LALT, KC_WH_L, KC_WH_R, KC_ACL0, KC_ACL1, KC_ACL2, KC_TRNS, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_RALT, KC_RGUI),

    [MOUSE_R] = LAYOUT(
        KC_TRNS, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD,
        KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_ESC , KC_RGUI, KC_WH_D, KC_MS_U, KC_WH_U, KC_ACL0, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_BTN5, KC_BTN4, KC_BTN2, KC_BTN3, KC_BTN1, KC_DEL , KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_ACL0, KC_ENT,
        KC_LSFT, KC_ACL2, KC_ACL1, KC_ACL0, KC_WH_L, KC_WH_R, KC_RALT, KC_WH_L, KC_BTN3, KC_WH_R, KC_TRNS, KC_RSFT, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_TRNS, KC_TRNS),

    [MIRROR] = LAYOUT(
        KC_GRV, KC_BSLS, KC_EQL, KC_MINS, KC_0, KC_9, KC_8, KC_7, KC_6, KC_5, KC_4, KC_3, KC_2, KC_1, KC_ESC,
        KC_BSPC, KC_P, KC_O, KC_I, KC_U, KC_Y, KC_T, KC_R, KC_E, KC_W, KC_Q, KC_LBRC, KC_RBRC, KC_TAB,
        RCTL_T(KC_ENT), KC_SCLN, KC_L, KC_K, KC_J, KC_H, KC_G, KC_F, KC_D, KC_S, KC_A, KC_TRNS, CTL_T(KC_ESC),
        KC_TRNS, KC_SLSH, KC_DOT, KC_COMM, KC_M, KC_N, KC_B, KC_V, KC_C, KC_X, KC_Z, KC_LSFT, KC_TRNS,
                                KC_RALT, KC_TRNS, KC_TRNS, KC_TRNS, KC_LALT),

    [SPACEFN] = LAYOUT(
        KC_GRV , KC_P1 , KC_P2  , KC_P3  , KC_P4  , KC_P5   , KC_P6  , KC_P7  , KC_P8  , KC_P9  , KC_P0   , KC_PMNS, KC_PEQL, KC_PSLS, RESET,
        GUI_T(KC_BSPC) , KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE , KC_TAB , KC_BSPC, KC_HOME, KC_PGUP, KC_PGDN , KC_END , KC_LCBR, KC_RCBR, KC_TAB,
        CTL_T(KC_ENT)  , KC_LPRN, KC_LEFT, KC_DOWN, KC_RIGHT, KC_BSLS, KC_LEFT, KC_DOWN, KC_UP  , KC_RIGHT, KC_COLN, KC_DQUO, KC_PENT,
        KC_TRNS        , KC_RPRN, KC_LCBR, KC_RCBR, KC_BSLS , KC_DEL , KC_GRV , KC_P0  , KC_LT  , KC_GT   , KC_QUES, KC_PDOT, KC_TRNS,
                                KC_RALT, KC_RGUI, KC_TRNS, KC_LGUI, KC_LALT),

    [FNKEYS] = LAYOUT(
        TO(BASE), KC_F1, KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_PIPE, KC_TILD,
        KC_TAB , KC_F7, KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, KC_LCBR, KC_RCBR, KC_BSPC,
        KC_LCTL, KC_F8, KC_TRNS, KC_BSPC, KC_DEL , KC_PIPE, KC_BSLS, KC_MINS, KC_EQL , KC_BSLS, KC_GRV , KC_DQUO, KC_ENT,
        KC_LSFT, KC_F9, KC_F10, KC_F11  , KC_F12 , KC_F13 , KC_F14 , KC_F15 , KC_LT  , KC_GT  , KC_QUES, KC_RSFT, KC_TRNS,
                                KC_LGUI , KC_LALT, KC_ENT , KC_RGUI, KC_RALT)};

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
  return 8; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static xtap xtap_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_scln_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_click_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_space_state = {
  .is_press_action = true,
  .state = 0
};

void click_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_click_state.state = cur_dance(state);
  switch (xtap_click_state.state) {
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
  switch (xtap_click_state.state) {
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
  xtap_click_state.state = 0;
}

void space_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_space_state.state = cur_dance(state);
  uint8_t lsft_mask = get_mods() & (MOD_BIT(KC_LSFT));
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        break;
    case SINGLE_HOLD:
        if (lsft_mask) {
            del_mods(MOD_BIT(KC_LSFT));
            layer_on(MIRROR);
            return;
        } else {
            layer_on(SPACEFN);
            return;
        }
        break;
    case DOUBLE_TAP:
        if (!state->interrupted && !lsft_mask) {
            register_code(KC_LCTL);
            register_code(KC_SPC);
            unregister_code(KC_SPC);
            unregister_code(KC_LCTL);
            return;

        }
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        break;
    case DOUBLE_HOLD:
        register_code(KC_LGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        register_code(KC_SPC);
    default:
        for (uint8_t idx = 0; idx < state->count; idx++) {
            register_code(KC_SPC);
            unregister_code(KC_SPC);
        }
  }
}

void space_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        break;
    case SINGLE_HOLD:
        layer_off(MIRROR);
        layer_off(SPACEFN);
        break;
    case DOUBLE_TAP:
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_LGUI);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SPC);
  }
  xtap_space_state.state = 0;
}

void scln_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_scln_state.state = cur_dance(state);
  switch (xtap_scln_state.state) {
    case SINGLE_TAP:
        register_code(KC_SCLN);
        break;
    case SINGLE_HOLD:
        layer_on(MOUSE_R);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_SCLN);
        break;
    case DOUBLE_HOLD:
        register_code(KC_RCTL);
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
        layer_off(MOUSE_R);
        break;
    case DOUBLE_TAP:
        del_weak_mods(KC_LSFT);
        unregister_code(KC_SCLN);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_RCTL);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_SCLN);
  }
  xtap_scln_state.state = 0;
}

void quote_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:
        register_code(KC_QUOT);
        unregister_code(KC_QUOT);
        break;
    case SINGLE_HOLD:
        layer_on(MIRROR);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_QUOT);
        break;
    case DOUBLE_HOLD:
        register_code(KC_RALT);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_QUOT);
        unregister_code(KC_QUOT);
        register_code(KC_QUOT);
  }
}

void quote_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP:
        break;
    case SINGLE_HOLD:
        layer_off(MIRROR);
        break;
    case DOUBLE_TAP:
        del_weak_mods(KC_LSFT);
        unregister_code(KC_QUOT);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_RALT);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_QUOT);
  }
  xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [CLICK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,space_finished,space_reset),
    [QUOTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,quote_finished,quote_reset),
    [SCLN]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL,scln_finished,scln_reset),
};

const uint16_t PROGMEM fn_actions[] = {
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t macro_kc = (keycode == MO(HHKB) ? DYN_REC_STOP : keycode);
    if (!process_record_dynamic_macro(macro_kc, record)) {
        return false;
    }

    switch (keycode) {
    default:
        return true;
    }

    return true;
}
