/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    BASE = 0,
    NUMPAD,
    FUNCTIONS,
    LAYER_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE


#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE


// Define combo key sequences
enum combo_events {
    COMBO_F2,
    COMBO_F3,
    COMBO_F4,
    COMBO_CAPS,
    COMBO_CAPS_WORD,
    COMBO_AT,
    COMBO_HASH,
    COMBO_LBRACKET,
    COMBO_RBRACKET,
    COMBO_UNDERSCORE,
    COMBO_PIPE,
    COMBO_PLUS,
    COMBO_EQUALS,
    COMBO_SQUARE_BRACKET,
    COMBO_TOGGLE_LAYER1_NM,
    COMBO_SPACE,
    COMBO_TOGGLE_LAYER1_SLASH,
    COMBO_LAYER3,
    COMBO_HYPER_SPACE,
};

// Define the key combinations for each combo
const uint16_t PROGMEM combo_f2[] = {KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM combo_f3[] = {KC_3, KC_4, COMBO_END};
const uint16_t PROGMEM combo_f4[] = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM combo_caps[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM combo_caps_word[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM combo_at[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM combo_hash[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM combo_lbracket[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM combo_rbracket[] = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM combo_underscore[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo_pipe[] = {KC_V, KC_B, COMBO_END};
const uint16_t PROGMEM combo_plus[] = {KC_9, KC_0, COMBO_END};
const uint16_t PROGMEM combo_equals[] = {KC_0, KC_MINUS, COMBO_END};
const uint16_t PROGMEM combo_square_bracket[] = {KC_P, KC_RBRC, COMBO_END};
const uint16_t PROGMEM combo_toggle_layer1_nm[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM combo_space[] = {KC_M, KC_COMMA, COMBO_END};
const uint16_t PROGMEM combo_toggle_layer1_slash[] = {KC_SLASH, SC_SENT, COMBO_END};
const uint16_t PROGMEM combo_layer3[] = {RGUI_T(KC_BSPC), HYPR_T(KC_DEL), COMBO_END};
const uint16_t PROGMEM combo_hyper_space[] = {KC_J, KC_K, COMBO_END};

// Map combos to their key sequences and result keys
combo_t key_combos[COMBO_COUNT] = {
    [COMBO_F2] = COMBO(combo_f2, KC_F2),
    [COMBO_F3] = COMBO(combo_f3, KC_F3),
    [COMBO_F4] = COMBO(combo_f4, KC_F4),
    [COMBO_CAPS] = COMBO(combo_caps, KC_CAPS),
    [COMBO_CAPS_WORD] = COMBO(combo_caps_word, 0x7c73),
    [COMBO_AT] = COMBO(combo_at, LALT(KC_2)),
    [COMBO_HASH] = COMBO(combo_hash, LALT(KC_3)),
    [COMBO_LBRACKET] = COMBO(combo_lbracket, LALT(KC_8)),
    [COMBO_RBRACKET] = COMBO(combo_rbracket, LALT(KC_9)),
    [COMBO_UNDERSCORE] = COMBO(combo_underscore, KC_INT1),
    [COMBO_PIPE] = COMBO(combo_pipe, LSFT(KC_INT1)),
    [COMBO_PLUS] = COMBO(combo_plus, LSFT(KC_EQUAL)),
    [COMBO_EQUALS] = COMBO(combo_equals, KC_EQUAL),
    [COMBO_SQUARE_BRACKET] = COMBO(combo_square_bracket, KC_LBRC),
    [COMBO_TOGGLE_LAYER1_NM] = COMBO(combo_toggle_layer1_nm, TG(1)),
    [COMBO_SPACE] = COMBO(combo_space, KC_SPACE),
    [COMBO_TOGGLE_LAYER1_SLASH] = COMBO(combo_toggle_layer1_slash, TG(1)),
    [COMBO_LAYER3] = COMBO(combo_layer3, MO(3)),  // Assuming momentary layer 3
    [COMBO_HYPER_SPACE] = COMBO(combo_hyper_space, HYPR(KC_SPACE)),
};


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_RBRC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LALT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, SC_SENT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                HYPR_T(KC_NO), LGUI_T(KC_SPC), LT(2,KC_ENT),   RGUI_T(KC_BSPC), LT(2,KC_NO),
                             LT(3,KC_LEFT), LCTL_T(KC_RGHT),   HYPR_T(KC_DEL)
  //                        ╰─----──────────────────────────╯ ╰────────────────╯
  ),

  [NUMPAD] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_NUBS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   LALT(KC_3), KC_INT1, LSFT(KC_8), LSFT(KC_9), LSFT(KC_INT1), KC_EQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   LSFT(KC_7),  KC_7,  KC_8,  KC_9,  KC_COMM,  KC_LBRC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LALT, KC_F11, KC_F12, LALT(KC_0), LALT(KC_8), LALT(KC_9), LSFT(KC_EQL), KC_4, KC_5, KC_6, KC_DOT, KC_NUHS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT, KC_MINS, LSFT(KC_2), LSFT(KC_3), LSFT(KC_4), LSFT(KC_5), KC_0, KC_1, KC_2, KC_3, KC_SLSH, SC_SENT, 
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                              HYPR_T(KC_NO), KC_LGUI, KC_ENT,  KC_BSPC, KC_SPC,
                                          KC_LALT, KC_LCTL,     RSFT(KC_0)
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [FUNCTIONS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_NO,   KC_NO,  KC_MPLY,  KC_MPRV,  KC_MNXT, KC_NO,    KC_1, KC_2, KC_3, KC_4, KC_5, RGUI(KC_BSPC),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_TAB, KC_NO, LGUI(KC_W), KC_BRID, KC_BRIU, LGUI(KC_T), RGUI(KC_T), RCS(KC_UP), KC_UP, RGUI(KC_O), RCS(KC_LEFT), KC_DEL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LALT,  KC_NO,  KC_MUTE,  KC_VOLD,  KC_VOLU, KC_NO,    HYPR(KC_H), KC_LEFT, KC_DOWN, KC_RGHT, KC_RALT, KC_RGUI,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT, LGUI(KC_Z), LGUI(KC_X), LGUI(KC_C), LGUI(KC_V), LGUI(KC_B), RGUI(KC_W), KC_SPC, RGUI(KC_C), RGUI(KC_V), RGUI(KC_Z), KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             HYPR_T(KC_NO), KC_LGUI, KC_ENT,    KC_BSPC, RGUI(KC_SPC),
                                         KC_LALT, MEH(KC_Q),    KC_TAB
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_NO, S_D_MOD, DPI_MOD, KC_NO, KC_NO, KC_NO, RCS(KC_1), RCS(KC_2), RCS(KC_3), RCS(KC_4), RCS(KC_5), RCS(KC_6),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_ON, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, RM_SPDU, KC_NO, RCS(KC_W), RCS(KC_E), RCS(KC_R), RCS(KC_T), RCS(KC_Y),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_TOGG, SNIPING, DRGSCRL, MS_BTN1, MS_BTN3, MS_BTN2, RCS(KC_A), RCS(KC_S), RCS(KC_D), RCS(KC_F), RCS(KC_G), RCS(KC_H),
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RM_OFF, RM_PREV, RM_HUED, RM_SATD, RM_VALD, RM_SPDD, RCS(KC_Z), RCS(KC_X), RCS(KC_C), RCS(KC_V), RCS(KC_B), RCS(KC_N),
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                 MS_BTN4, MS_BTN5, MS_BTN6,     MS_BTN2, MS_BTN1,
                                          KC_NO, MS_BTN7,       MS_BTN3
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
