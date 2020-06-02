#include QMK_KEYBOARD_H

// Layer names
#define BASE 0
#define HHKB 1
#define MOUSEL 2
#define MOUSER 3
#define MIRROR 4
#define SPACE 5
#define OTHER 6

// Required for leader function. Measured in ms
// #define LEADER_TIMEOUT 300

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* BASE Level: Default Layer
     |-----------+-------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+------+----|
     |     `     |  1    | 2      | 3     |   4   |   5   |   6   | 7     | 8     | 9     | 0     | -     | =     | Bksp |Lead|
     |-----------+-------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+------+----|
     |    Tab    |  Q    | W      | E     |   R   |   T   |   Y   |  U    | I     | O     | P     | [     | ]     |      | \  |
     |-----------+-------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+------+----|
     | Esc/Hyper |   A   | S      | D     |   F   |   G   |   H   |  J    | K     | L     |;/Media| '     | Enter |      |    |
     |-----------+-------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+------+----|
     |   Shift   | Z/Ctl | X/Alt  | C/Gui |   V   |   B   |   N   |  M    | ,/Gui | ./Alt | //Ctl | Shift | Dev   |      |    |
     |-----------+-------+--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+------+----|
     TODO: Maybe add a photoshop layer for when I need to hold spacebar down. Maybe just make it a layer that you
                 |------+------+-----------------------+------+------|
                 | Dev  |Mouse | ******* Space ******* | Dev  |Mouse |
                 |------+------+-----------------------+------+------|
    */

    [BASE] = LAYOUT( //  default layer
        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSLS, KC_GRV,
        GUI_T(KC_TAB), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSPC,
        CTL_T(KC_ESC), LT(MOUSEL, KC_A), LT(OTHER, KC_S), KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, LT(MOUSER, KC_SCLN), KC_QUOT, MT(KC_RCTL, KC_ENT),
        OSM(MOD_LSFT), GUI_T(KC_Z), KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, MT(KC_RGUI, KC_SLSH), MT(KC_RSFT, KC_ESC), TT(HHKB),
        KC_LALT, LT(MIRROR, KC_GRV), LT(SPACE, KC_SPC), TT(MIRROR), KC_RALT),

    /* Layer HHKB: HHKB mode (HHKB Fn)
      TODO: Add a cmd/tab function to the developer layer for quick switching between different applications when debugging
      |------+-----+-----+-----+----+----+--------+----------------+--------------+-----------+-------------+-----+-------+-------+-----|
      |      |     |     |     |    |    |        |                |              |           |             |     |       |       |     |
      |------+-----+-----+-----+----+----+--------+----------------+--------------+-----------+-------------+-----+-------+-------+-----|
      |      |     |     |     |    |    |        |                |              |           |             |     |       |       |     |
      |------+-----+-----+-----+----+----+--------+----------------+--------------+-----------+-------------+-----+-------+-------+-----|
      |      | F1  | F2  | F3  | F4 | F5 | Left   |  Down          | Up           | Right     |             |     |       |       |     |
      |------+-----+-----+-----+----+----+--------+----------------+--------------+-----------+-------------+-----+-------+-------+-----|
      |      |     |     |     |    |    |        |                |              |           |             |     |       |       |     |
      |------+-----+-----+-----+----+----+--------+----------------+--------------+-----------+-------------+-----+-------+-------+-----|

                 |------+------+----------------------+------+------+
                 | **** | **** | ******************** | **** | **** |
                 |------+------+----------------------+------+------+
     */

    [HHKB] = LAYOUT(
        KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_INS, KC_DEL,
        KC_CAPS, KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_PSCR, KC_SLCK, KC_PAUS, KC_UP, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_PAST, KC_PSLS, KC_HOME, KC_PGUP, KC_LEFT, KC_RGHT, KC_ENT,
        KC_LSFT, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24, KC_PPLS, KC_PMNS, KC_END, KC_PGDN, KC_DOWN, KC_RSFT, KC_TRNS,
        KC_LALT, KC_LGUI, KC_LGUI, KC_RGUI, KC_RALT),

    [MOUSEL] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TAB, KC_TRNS, KC_WH_U, KC_MS_U, KC_WH_D, KC_LGUI, KC_ESC, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_TRNS, KC_TRNS, KC_BSPC,
        KC_ACL0, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_LCTL, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_TRNS, KC_TRNS, KC_ENT,
        KC_LSFT, KC_ACL0, KC_TRNS, KC_BTN3, KC_BTN2, KC_LALT, KC_WH_L, KC_WH_R, KC_ACL0, KC_ACL1, KC_ACL2, KC_TRNS, KC_TRNS,
        KC_LGUI, KC_LALT, KC_BTN1, KC_RALT, KC_RGUI),

    [MOUSER] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_ESC, KC_RGUI, KC_WH_D, KC_MS_U, KC_WH_U, KC_ACL0, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_LCTL, KC_TRNS, KC_LEFT, KC_UP, KC_DOWN, KC_RIGHT, KC_RCTL, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS, KC_ACL0, KC_ENT,
        KC_LSFT, KC_ACL0, KC_ACL1, KC_ACL2, KC_WH_L, KC_WH_R, KC_RALT, KC_BTN2, KC_BTN3, KC_TRNS, KC_TRNS, KC_RSFT, KC_TRNS,
        KC_LGUI, KC_LALT, KC_BTN1, KC_TRNS, KC_TRNS),

    [MIRROR] = LAYOUT(
        KC_GRV, KC_BSLS, KC_EQL, KC_MINS, KC_0, KC_9, KC_8, KC_7, KC_6, KC_5, KC_4, KC_3, KC_2, KC_1, KC_ESC,
        KC_BSPC, KC_P, KC_O, KC_I, KC_U, KC_Y, KC_T, KC_R, KC_E, KC_W, KC_Q, KC_LBRC, KC_RBRC, KC_TAB,
        RCTL_T(KC_ENT), KC_SCLN, KC_L, KC_K, KC_J, KC_H, KC_G, KC_F, KC_D, KC_S, KC_A, KC_QUOT, CTL_T(KC_ESC),
        KC_RSFT, KC_SLSH, KC_DOT, KC_COMM, KC_M, KC_N, KC_B, KC_V, KC_C, KC_X, KC_Z, KC_LSFT, KC_TRNS,
        KC_RALT, KC_RGUI, KC_SPC, KC_TRNS, KC_LALT),

    [SPACE] = LAYOUT(
        KC_BSLS, KC_EQL, KC_MINS, KC_0, KC_9, KC_8, KC_7, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_TRNS,
        GUI_T(KC_BSPC), KC_LBRC, KC_RBRC, KC_UP, S(KC_BSLS), KC_BSLS, KC_BSPC, KC_P4, KC_P5, KC_P6, KC_PENT, KC_PAST, KC_PSLS, KC_TAB,
        CTL_T(KC_ENT), S(KC_9), KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGUP, KC_HOME, KC_P1, KC_P2, KC_P3, KC_MINS, KC_EQL, KC_ENT,
        TT(KC_LSFT), S(KC_0), S(KC_LBRC), S(KC_RBRC), KC_PGDN, KC_END, KC_DEL, KC_P0, KC_PEQL, KC_PDOT, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_RALT, KC_RGUI, KC_TRNS, KC_LGUI, KC_LALT),

    [OTHER] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_TRNS,
        KC_TAB, KC_F7, KC_F8, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSPC,
        KC_LCTL, KC_F9, KC_TRNS, KC_BSPC, KC_DEL, KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_MUTE, KC_TRNS, KC_MPLY, KC_ENT,
        KC_LSFT, KC_F10, KC_F11, KC_F12, KC_F13, KC_F14, KC_TRNS, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, KC_RSFT, KC_TRNS,
        KC_LGUI, KC_LALT, KC_ENT, KC_RGUI, KC_RALT)};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // MACRODOWN only works in this function
    switch (id)
    {
    case 0:
        if (record->event.pressed)
        {
            register_code(KC_RSFT);
        }
        else
        {
            unregister_code(KC_RSFT);
        }
        break;
    }
    return MACRO_NONE;
};
