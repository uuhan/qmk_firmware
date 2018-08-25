#include "xd75.h"

#include "quantum.h"
#include "action.h"
#include "mousekey.h"
#include "process_keycode/process_tap_dance.h"

extern keymap_config_t keymap_config;

void send_keycode(uint16_t kc) {
    register_code(kc);
    unregister_code(kc);
}


#ifdef IS_COLINTA
#include "secrets.h"
#else
#define SENDSTRING_MM0 ""
#define SENDSTRING_MM1 ""
#define SENDSTRING_MM2 ""
#endif

#define GOTO_FN TT(LAYER_FN)

// - sticky keys, aka one-shot
#define OSCTL OSM(MOD_LCTL)
#define OSALT OSM(MOD_LALT)
#define OSSFT OSM(MOD_LSFT)
#define OSGUI OSM(MOD_LGUI)

// "MMENU" is a macro for "CMD+SPC" (aka Spotlight/Alfred)
#define MMENU LGUI(KC_SPC)
#define _____ KC_TRNS
#define MM_0 DYN_MACRO_PLAY1
#define MM_1 DYN_MACRO_PLAY2

enum layers {
    LAYER_QWERTY,
    LAYER_MACRO,
    LAYER_MOUSE_L,
    LAYER_MOUSE_R,
    LAYER_SPACEFN,
    LAYER_COLEMAK,
    LAYER_FN,
};

enum my_keycods {
    TH_M0 = SAFE_RANGE,
    TH_M1,
    TH_M2,
    TH_F1,
    TH_F2,
    TH_F3,
    TH_F4,
    TH_F5,
    TH_F6,
    TH_F7,
    TH_F8,
    TH_F9,
    TH_F10,

    TH_COMM,
    TH_DOT,

    TH_LAST,

    MM_2,
    DYNAMIC_MACRO_RANGE,
};

enum {
    TD_CLICK = 0,
    TD_SPACE,
};

// tap-hold settings
#define LONGPRESS_DELAY 250
static const int TH_EVENTS_COUNT = TH_LAST - TH_M0;


#include "dynamic_macro.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [LAYER_QWERTY] = KEYMAP(
    ALT_T(KC_ESC), TH_F1                   , TH_F2             , TH_F3          , TH_F4       , TH_F5                    , MM_0           , SH_TT       , MM_1          , TH_F6 , TH_F7  , TH_F8  , TH_F9  , TH_F10                    , DYN_MACRO_PLAY1      ,
    GUI_T(KC_TAB), KC_Q                    , KC_W              , KC_E           , KC_R        , KC_T                     , KC_MINS        , KC_BSLS     , KC_EQL        , KC_Y  , KC_U   , KC_I   , KC_O   , KC_P                      , KC_BSPC              ,
    CTL_T(KC_ESC), LT(LAYER_MOUSE_L, KC_A) , LT(LAYER_FN, KC_S), KC_D           , KC_F        , KC_G                     , KC_QUOT        , _____       , KC_DQUO       , KC_H  , KC_J   , KC_K   , KC_L   , LT(LAYER_MOUSE_R, KC_SCLN), RCTL_T(KC_ENT)       ,
    OSM(MOD_LSFT), GUI_T(KC_Z)             , CTL_T(KC_X)       , ALT_T(KC_C)    , KC_V        , KC_B                     , DYN_REC_START1, DYN_REC_STOP, DYN_MACRO_PLAY1, KC_N  , KC_M   , KC_COMM, KC_DOT , RGUI_T(KC_SLSH)           , MT(MOD_RSFT, KC_QUOT),
    KC_LALT      , MO(LAYER_MACRO)         , DYN_REC_START1    , DYN_MACRO_PLAY1, SH_T(KC_TAB), TD(TD_SPACE)             , KC_ENT         , KC_BSPC     , KC_SPC        , KC_ENT, KC_MINS, KC_EQL , KC_LBRC, KC_RBRC                   , _____
  ),

  [LAYER_MACRO] = KEYMAP(
    RESET, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _____ , _____, _____  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _____, _____  , KC_HOME, KC_PGUP, KC_PGDN, KC_END , _____ , _____, _____  , KC_RGUI, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____ , _____, _____  , _____  , _____  , _____  , _____  , _____  , _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____ , _____, _____  , KC_RALT, _____  , _____  , _____  , _____  , _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____ , _____, _____  , _____  , _____  , _____  , _____  , _____  , _____
  ),

  [LAYER_MOUSE_L] = KEYMAP(
    _____, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC     ,RGB_HUD       , RGB_RMOD, RGB_HUI        , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _____, KC_Q   , KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI     ,RGB_SAD       , RGB_TOG , RGB_SAI        , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _____  ,
    _____, _____  , KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL     ,RGB_VAD       , _____   , RGB_VAI        , KC_H   , KC_BTN1, KC_BTN3, KC_BTN2, KC_BTN4, KC_BTN5,
    _____, KC_Z   , KC_X   , KC_C   , KC_V   , KC_LALT     ,DYN_REC_START2, _____   , DYN_MACRO_PLAY2, KC_N   , KC_M   , _____  , _____  , _____  , _____  ,
    _____, _____  , _____  , _____  , KC_BTN2, TD(TD_CLICK),KC_BTN3 , _____  , _____  , _____  , _____  , _____  , _____  , _____  , _____
  ),

  [LAYER_MOUSE_R] = KEYMAP(
    _____  , KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _____  , _____, _____  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _____  , _____  , _____  , _____  , _____  , _____  , _____  , _____, _____  , KC_RGUI, KC_WH_D, KC_MS_U, KC_WH_U, _____  , _____  ,
    KC_ACL2, KC_ACL0, KC_BTN2, KC_BTN3, KC_BTN1, _____  , _____  , _____, _____  , KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R, _____  , _____  ,
    _____  , _____  , _____  , _____  , _____  , _____  , _____  , _____, _____  , KC_RALT, _____  , _____  , _____  , _____  , _____  ,
    _____  , _____  , _____  , _____  , _____  , _____  , _____  , _____, KC_BTN3, KC_BTN1, KC_BTN2, _____  , _____  , _____  , _____
  ),

  [LAYER_SPACEFN] = KEYMAP(
    RESET, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _____  , _____, _____  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _____, KC_LBRC, KC_RBRC, KC_UP  , KC_PIPE, KC_TAB , _____  , _____, _____  , KC_RGUI, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, _____  ,
    _____, KC_LPRN, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSLS, _____  , _____, _____  , KC_RCTL, KC_MINS, KC_EQL , KC_BSLS, KC_GRV , _____  ,
    _____, KC_RPRN, KC_LCBR, KC_RCBR, _____  , _____  , _____  , _____, _____  , KC_RALT, _____  , _____  , _____  , _____  , _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____  , _____, KC_BTN3, KC_BTN1, KC_BTN2, _____  , _____  , _____  , _____
  ),

/* COLEMAK
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | ESC    | 1      | 2      | 3      | 4      | 5      | M(0)   | M(1)   | DEL    | 6      | 7      | 8      | 9      | 0      | -      |
 * |--------+-/F1----+-/F2----+-/F3----+-/F4----+-/F5----+--------+--------+--------+-/F6----+-/F7----+-/F8----+-/F9----+-/F10---+--------|
 * | TAB    | Q      | W      | F      | P      | G      | M(2)   |  (FN)  | BKSP   | J      | L      | U      | Y      | ;      | =      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | SHIFT  | A      | R      | S      | T      | D      | [      | ]      | ENTER  | H      | N      | E      | I      | O      | '      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | CTRL   | Z      | X      | C      | V      | B      | HOME   | END    | PG UP  | K      | M      | ,      | .      | /      | \      |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | VOL-   | PLAY   | VOL+   | ALT    | GUI    |          SPACE           | PG DN  | MENU   | ESC    | LEFT   | DOWN   | UP     | RIGHT  |
 * '--/RRND----/MUTE----/FFWD-------------------------------------------------------------------------------------------------------------'
 */

  [LAYER_COLEMAK] = KEYMAP(
    KC_GRV, TH_F1, TH_F2, TH_F3, TH_F4, TH_F5,  MM_0,    MM_1,    KC_DEL,  TH_F6, TH_F7,  TH_F8,   TH_F9,   TH_F10,  KC_MINS,
    KC_TAB, KC_Q,  KC_W,  KC_F,  KC_P,  KC_G,   MM_2,    GOTO_FN, KC_BSPC, KC_J,  KC_L,   KC_U,    KC_Y,    KC_SCLN, KC_EQL,
    OSCTL,  KC_A,  KC_R,  KC_S,  KC_T,  KC_D,   KC_LBRC, KC_RBRC, KC_ENT,  KC_H,  KC_N,   KC_E,    KC_I,    KC_O,    KC_QUOT,
    OSSFT,  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,   KC_HOME, KC_END,  KC_PGUP, KC_K,  KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS,
    TH_M0,  TH_M1,  TH_M2, OSALT, OSGUI, _____ , KC_SPC,  _____ ,  KC_PGDN, MMENU, KC_ESC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
  ),

  [LAYER_FN] = KEYMAP(
    RESET, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _____ , _____, _____  , KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
    _____, _____  , KC_HOME, KC_PGUP, KC_PGDN, KC_END , _____ , _____, _____  , KC_RGUI, KC_UNDS, KC_PLUS, KC_PIPE, KC_TILD, _____  ,
    _____, _____  , _____  , KC_BSPC, KC_DEL , _____  , _____ , _____, _____  , KC_RCTL, KC_MINS, KC_EQL , KC_BSLS, KC_GRV , _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____ , _____, _____  , KC_RALT, _____  , _____  , _____  , _____  , _____  ,
    _____, _____  , _____  , _____  , _____  , _____  , _____ , _____, _____  , _____  , _____  , _____  , _____  , _____  , _____
  ),
};

typedef struct {
    bool is_pressed;
    uint16_t timer;
    uint16_t kc_tap;
    uint16_t kc_hold;
} tap_hold_t;

static tap_hold_t th_events[] = {
    { .is_pressed = false, .timer = 0, .kc_tap = KC_VOLD, .kc_hold = KC_MRWD }, // TH_M0
    { .is_pressed = false, .timer = 0, .kc_tap = KC_MPLY, .kc_hold = KC_MUTE }, // TH_M1
    { .is_pressed = false, .timer = 0, .kc_tap = KC_VOLU, .kc_hold = KC_MFFD }, // TH_M2

    { .is_pressed = false, .timer = 0, .kc_tap = KC_1, .kc_hold = KC_F1 }, // TH_F1
    { .is_pressed = false, .timer = 0, .kc_tap = KC_2, .kc_hold = KC_F2 }, // TH_F2
    { .is_pressed = false, .timer = 0, .kc_tap = KC_3, .kc_hold = KC_F3 }, // TH_F3
    { .is_pressed = false, .timer = 0, .kc_tap = KC_4, .kc_hold = KC_F4 }, // TH_F4
    { .is_pressed = false, .timer = 0, .kc_tap = KC_5, .kc_hold = KC_F5 }, // TH_F5
    { .is_pressed = false, .timer = 0, .kc_tap = KC_6, .kc_hold = KC_F6 }, // TH_F6
    { .is_pressed = false, .timer = 0, .kc_tap = KC_7, .kc_hold = KC_F7 }, // TH_F7
    { .is_pressed = false, .timer = 0, .kc_tap = KC_8, .kc_hold = KC_F8 }, // TH_F8
    { .is_pressed = false, .timer = 0, .kc_tap = KC_9, .kc_hold = KC_F9 }, // TH_F9
    { .is_pressed = false, .timer = 0, .kc_tap = KC_0, .kc_hold = KC_F10}, // TH_F10

    { .is_pressed = false, .timer = 0, .kc_tap = KC_COMM, .kc_hold = KC_MINS}, // TH_MINS
    { .is_pressed = false, .timer = 0, .kc_tap = KC_DOT , .kc_hold = KC_EQL }, // TH_DOT
};

void taphold_tapped(uint8_t index, bool pressed) {
    if (index >= TH_EVENTS_COUNT) { return; }

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

void matrix_scan_user() {
    for (uint8_t index = 0 ; index < TH_EVENTS_COUNT ; ++index ) {
        tap_hold_t *th_event = &th_events[index];
        if ( th_event->is_pressed && timer_elapsed(th_event->timer) > LONGPRESS_DELAY) {
            register_code(th_event->kc_hold);
            unregister_code(th_event->kc_hold);
            th_event->is_pressed = false;
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t macro_kc = (keycode == MO(LAYER_MACRO) ? DYN_REC_STOP : keycode);
    if (!process_record_dynamic_macro(macro_kc, record)) {
        return false;
    }

    switch (keycode) {
        case TH_M0 ... TH_LAST:
            taphold_tapped(keycode - TH_M0, record->event.pressed);
            return false;
        default:
            return true;
    }

    return true;
}

const keypos_t hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
  {{14, 0}, {13, 0}, {12, 0}, {11, 0}, {10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}},
  {{14, 1}, {13, 1}, {12, 1}, {11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
  {{14, 2}, {13, 2}, {12, 2}, {11, 2}, {10, 2}, {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
  {{14, 3}, {13, 3}, {12, 3}, {11, 3}, {10, 3}, {9, 3}, {8, 3}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}},
  {{14, 4}, {13, 4}, {12, 4}, {11, 4}, {10, 4}, {9, 4}, {8, 4}, {7, 4}, {6, 4}, {5, 4}, {4, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}},
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

typedef struct {
    bool is_press_action;
    bool is_keeping;
    int state;
} xtap;

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

static xtap xtap_cick_state = {
  .is_press_action = true,
  .state = 0
};

static xtap xtap_space_state = {
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

void space_finished (qk_tap_dance_state_t *state, void *user_data) {
  xtap_space_state.state = cur_dance(state);
  switch (xtap_space_state.state) {
    case SINGLE_TAP:
        send_keycode(KC_SPC);
        break;
    case SINGLE_HOLD:
        layer_on(LAYER_SPACEFN);
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
        layer_off(LAYER_SPACEFN);
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

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CLICK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,click_finished,click_reset),
    [TD_SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL,space_finished,space_reset),
};
