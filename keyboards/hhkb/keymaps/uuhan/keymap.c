#include QMK_KEYBOARD_H

#include "quantum.h"
#include "action.h"
#include "mousekey.h"
#include "process_keycode/process_tap_dance.h"
#include "process_keycode/process_leader.h"

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
    TRIPLE_TAP = 6,
    TRIPLE_HOLD = 7
};

enum {
    CLICK = 0,
    SPACE,
    QUOTE,
    SCLN,
    GRAVE,
    SLSH,
    PIPE,
    LSFT,
};

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
        KC_ESC, KC_1 , KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_LEAD,
        GUI_T(KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
        CTL_T(KC_ESC), LT(MOUSE_L, KC_A), LT(FNKEYS, KC_S), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, TD(SCLN), TD(QUOTE), MT(MOD_RCTL, KC_ENT),
        TD(LSFT)     , GUI_T(KC_Z), KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, TD(SLSH), MT(MOD_RSFT, KC_ESC), TT(HHKB),
                        KC_LALT, TD(GRAVE), TD(SPACE), TT(MIRROR), KC_RALT),

    [HHKB] = LAYOUT(
        KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_ENT,
        RESET  , KC_F20, KC_F21, KC_F22, KC_F23, KC_F24, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_RSFT, KC_TRNS,
                                KC_LALT, KC_LGUI, KC_LGUI, KC_RGUI, KC_RALT),

    [MOUSE_L] = LAYOUT(
        KC_TRNS, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD,
        KC_TAB , KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI, KC_ESC , KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_TRNS, KC_TRNS, KC_BSPC,
        KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL, KC_BSPC, KC_BTN1, KC_BTN3, KC_BTN2, KC_BTN4, KC_BTN5, KC_ACL0,
        KC_LSFT, KC_ACL0, KC_TRNS, KC_BTN3, KC_BTN2, KC_LALT, KC_WH_L, KC_WH_R, KC_ACL0, KC_ACL1, KC_ACL2, KC_TRNS, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_RALT, KC_RGUI),

    [MOUSE_R] = LAYOUT(
        KC_TRNS, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD,
        KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_ESC , KC_RGUI, KC_WH_D, KC_MS_U, KC_WH_U, KC_ACL0, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_BTN5, KC_BTN4, KC_BTN2, KC_BTN3, KC_BTN1, KC_DEL , KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_ACL0, KC_ENT,
        KC_LSFT, KC_ACL2, KC_ACL1, KC_ACL0, KC_WH_L, KC_WH_R, KC_RALT, KC_BTN2, KC_BTN3, KC_TRNS, KC_TRNS, KC_RSFT, KC_TRNS,
                                KC_LGUI, KC_LALT, TD(CLICK), KC_TRNS, KC_TRNS),

    [MIRROR] = LAYOUT(
        KC_GRV, KC_BSLS, KC_EQL, KC_MINS, KC_0, KC_9, KC_8, KC_7, KC_6, KC_5, KC_4, KC_3, KC_2, KC_1, KC_ESC,
        KC_BSPC, KC_P, KC_O, KC_I, KC_U, KC_Y, KC_T, KC_R, KC_E, KC_W, KC_Q, KC_LBRC, KC_RBRC, KC_TAB,
        RCTL_T(KC_ENT), KC_SCLN, KC_L, KC_K, KC_J, KC_H, KC_G, KC_F, KC_D, KC_S, KC_A, KC_TRNS, CTL_T(KC_ESC),
        KC_RSFT, KC_SLSH, KC_DOT, KC_COMM, KC_M, KC_N, KC_B, KC_V, KC_C, KC_X, KC_Z, KC_LSFT, KC_TRNS,
                                KC_RALT, KC_TRNS, KC_SPC, KC_TRNS, KC_LALT),

    [SPACEFN] = LAYOUT(
        KC_BSLS, KC_EQL, KC_MINS, KC_0   , KC_9   , KC_8    , KC_7   , KC_P7  , KC_P8, KC_P9  , KC_PAST, KC_PMNS, KC_PPLS, KC_PSLS, RESET,
        GUI_T(KC_BSPC) , KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE , KC_TAB , KC_BSPC, KC_P4, KC_P5  , KC_P6  , KC_PEQL, KC_LCBR, KC_RCBR, KC_TAB,
        CTL_T(KC_ENT)  , KC_LPRN, KC_LEFT, KC_DOWN, KC_RIGHT, KC_BSLS, KC_P0  , KC_P1, KC_P2  , KC_P3  , KC_COLN, KC_DQUO, KC_PENT,
        F(0)           , KC_RPRN, KC_LCBR, KC_RCBR, KC_BSLS , KC_DEL , KC_GRV , KC_P0, KC_LT  , KC_GT  , KC_QUES, KC_PDOT, KC_TRNS,
                                KC_RALT, KC_RGUI, KC_TRNS, KC_LGUI, KC_LALT),

    [FNKEYS] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_PIPE, KC_TILD,
        KC_TAB , KC_F7, KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END , KC_MINS, KC_EQL , KC_BSLS, KC_GRV , KC_LCBR, KC_RCBR, KC_BSPC,
        KC_LCTL, KC_F8, KC_TRNS, KC_BSPC, KC_DEL , KC_PIPE, KC_BSLS, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, KC_DQUO, KC_ENT,
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

static xtap xtap_slsh_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_lsft_state = {
  .is_press_action = true,
  .is_keeping = false,
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
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        register_code(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_on(SPACEFN);
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
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_off(SPACEFN);
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

void grave_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:
        register_code(KC_GRV);
        break;
    case SINGLE_HOLD:
        layer_on(MIRROR);
        break;
    case DOUBLE_TAP:
        add_weak_mods(MOD_LSFT);
        register_code(KC_GRV);
        break;
    case DOUBLE_HOLD:
        register_code(KC_GRV);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_GRV);
        unregister_code(KC_GRV);
        register_code(KC_GRV);
  }
}

void grave_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP:
        unregister_code(KC_GRV);
        break;
    case SINGLE_HOLD:
        layer_off(MIRROR);
        break;
    case DOUBLE_TAP:
        del_weak_mods(KC_LSFT);
        unregister_code(KC_GRV);
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_GRV);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_GRV);
  }
  xtap_state.state = 0;
}

void quote_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP:
        register_code(KC_QUOT);
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
        unregister_code(KC_QUOT);
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
        del_weak_mods(KC_LSFT);
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

void lsft_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_lsft_state.state = cur_dance(state);
  switch (xtap_lsft_state.state) {
    case SINGLE_TAP:
        // xtap_lsft_state.timestamp = state->timer;
        register_code(KC_LSFT);
        break;
    case SINGLE_HOLD:
        register_code(KC_LSFT);
        break;
    case DOUBLE_TAP:
        if (xtap_lsft_state.is_keeping) {
            xtap_lsft_state.is_keeping = false;
            unregister_code(KC_LSFT);
        } else {
            xtap_lsft_state.is_keeping = true;
            register_code(KC_LSFT);
        }
        break;
    case DOUBLE_HOLD:
        register_code(KC_LSFT);
        break;
    case DOUBLE_SINGLE_TAP:
        register_code(KC_LSFT);
        unregister_code(KC_LSFT);
        register_code(KC_LSFT);
  }
}

void lsft_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_lsft_state.state) {
    case SINGLE_TAP:
        if (!state->interrupted) {
            register_code(KC_LCTL);
            register_code(KC_SPC);
            unregister_code(KC_SPC);
            unregister_code(KC_LCTL);
        }
        unregister_code(KC_LSFT);
        break;
    case SINGLE_HOLD:
        unregister_code(KC_LSFT);
        break;
    case DOUBLE_TAP:
        break;
    case DOUBLE_HOLD:
        unregister_code(KC_LSFT);
        break;
    case DOUBLE_SINGLE_TAP:
        unregister_code(KC_LSFT);
  }
  xtap_lsft_state.state = 0;
}

void pipe_dance(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("|");
            reset_tap_dance(state);
            break;
        case 2:
            SEND_STRING("\\");
            reset_tap_dance(state);
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [CLICK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,space_finished,space_reset),
    [QUOTE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,quote_finished,quote_reset),
    [SCLN]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL,scln_finished,scln_reset),
    [GRAVE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,grave_finished,grave_reset),
    [SLSH]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL,slsh_finished,slsh_reset),
    [PIPE]  = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, pipe_dance, NULL, 200),
    [LSFT]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL,lsft_finished,lsft_reset),
};

LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        // Firefox Keyshots
        SEQ_ONE_KEY(KC_Q) {
            SEND_STRING(SS_LALT(SS_LGUI("c")));
        }
        SEQ_ONE_KEY(KC_W) {
            SEND_STRING(SS_LALT(SS_LGUI("k")));
        }
        SEQ_ONE_KEY(KC_E) {
            SEND_STRING(SS_LALT(SS_LGUI("e")));
        }
        SEQ_ONE_KEY(KC_R) {
            SEND_STRING(SS_LSFT(SS_TAP(X_F9)));
        }
    }
}

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_MODS_TAP_TOGGLE(MOD_LSFT),
};
