/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

static void render_logo(void);

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap-dance keycodes
enum {
    TD_THSL     // Tap dance: tap-hold symbol layer
};

td_state_t cur_dance(tap_dance_state_t *state);
void thsl_finished(tap_dance_state_t *state, void *user_data);
void thsl_reset(tap_dance_state_t *state, void *user_data);

enum layer_names {
    _BL,    // Base Layer
    _SNL,   // Symbol-Number Layer
    _NL,    // Navigation Layer
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,                         KC_F,    KC_G,    KC_C,    KC_R,    KC_L, KC_SLSH,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_CAPS,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                         KC_D,    KC_H,    KC_T,    KC_N,    KC_S, KC_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_SCLN,    KC_Q,    KC_J,    KC_K,    KC_X,                         KC_B,    KC_M,    KC_W,    KC_V,    KC_Z, KC_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                         KC_LGUI,TD(TD_THSL),KC_SPC,    KC_BSPC, KC_LCTL, TG(_NL)
                                      //`--------------------------'  `--------------------------'

  ),

    [_SNL] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_ESC, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC,    KC_7,    KC_8,    KC_9, KC_MINS, KC_UNDS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN,  KC_EQL,                      KC_AMPR,    KC_4,    KC_5,    KC_6, KC_PLUS, KC_ASTR,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,  KC_GRV, KC_TILD, KC_LCBR, KC_RCBR, KC_PIPE,                         KC_0,    KC_1,    KC_2,    KC_3, KC_BSLS, KC_SLSH,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LSFT, KC_TRNS,  KC_SPC,     KC_ENT, KC_LGUI, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [_NL] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_ACL0, XXXXXXX, KC_WH_D, KC_WH_U, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, KC_PSCR,                      KC_ACL1, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_ACL2, KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_RALT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, KC_TRNS,  KC_SPC,    KC_BTN1, KC_BTN2, TG(_NL)
                                      //`--------------------------'  `--------------------------'
  )
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1 && state->pressed) {
        return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

static td_tap_t thsl_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void thsl_finished(tap_dance_state_t *state, void *user_data) {
    thsl_tap_state.state = cur_dance(state);
    switch (thsl_tap_state.state) {
        case TD_SINGLE_HOLD:
            layer_on(_SNL);
            break;
        case TD_DOUBLE_TAP:
            if (layer_state_is(_SNL)) {
                layer_off(_SNL);
            } else {
                layer_on(_SNL);
            }
            break;
        default:
            break;
    }
}

void thsl_reset(tap_dance_state_t *state, void *user_data) {
    if (thsl_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_SNL);
    }
    thsl_tap_state.state = TD_NONE;
}

// Associate tap dance keys with their funcionality.
tap_dance_action_t tap_dance_actions[] = {
    [TD_THSL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, thsl_finished, thsl_reset)
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 225;
        default:
            return TAPPING_TERM;
    }
}

#ifdef OLED_ENABLE

static void render_logo() {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}
bool oled_timed_out = false;

bool oled_task_user() {
    // Screensaver code
    // Turn off oled after 10 minutes.
    if (is_oled_on() && last_input_activity_elapsed() >= OLED_TIMEOUT) {
        oled_timed_out = true;
    }

    if (oled_timed_out && last_input_activity_elapsed() < OLED_TIMEOUT) {
        oled_timed_out = false;
        oled_on();
    }

    if (oled_timed_out) {
        oled_off();
        return false;
    }

    // If not screen saver mode, do other stuff.
    if (!is_keyboard_master()) {
        render_logo();
    } else {
    // Layer State
        switch (get_highest_layer(layer_state)) {
            case _BL:
                oled_write_P(PSTR("BASE"), false);
                break;
            case _SNL:
                oled_write_P(PSTR("SYMB"), false);
                break;
            case _NL:
                oled_write_P(PSTR("NAVI"), false);
                break;
            default:
                oled_write_ln_P(PSTR("????"), false);
        }

        // Separator
        oled_write_P(PSTR(" | "), false);

        // CAPS LOCK State
        led_t led_state = host_keyboard_led_state();
        oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);

        // Next Line
        oled_write_P(PSTR("\n"), false);

        // Words/Characters Per Minute
        oled_write_P(PSTR("WPM: "), false);
        oled_write(get_u8_str(get_current_wpm(), ' '), false);
        oled_write_P(PSTR(" | CPM: "), false);
        oled_write(get_u16_str(((uint16_t)get_current_wpm() * 5), ' '), false);
    }

    return false;
}
#endif
