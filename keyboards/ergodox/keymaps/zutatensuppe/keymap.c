#include <stdarg.h>
#include "ergodox.h"
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "timer.h"
#include "keymap_plover.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h" 

/*
 * Bu-TECK inspired Layout. Credit to Oobly
 * https://geekhack.org/index.php?topic=49721.msg1078758#msg1078758
 *
 * NOTES:
 * To be used with QWERTY US 101-key keyboard layout
 *
 * ACTION_MODS_TAP_KEY(MOD_RCTL, KC_ENT)
 * Works as a modifier key while holding, but registers a key on tap(press and release quickly)
 * From the docs it seems like the destination layer button must be the same or transparent.
 *
 * ACTION_LAYER_TAP_KEY(layer, key)
 * Turns on layer momentary while holding, but registers key on tap(press and release quickly).
 *
 * MACRO()
 *
 */

// layer aliases
#define L0_DEF 0 //
#define L1_LFT 1 //
#define L2_RGT 2 //
#define L3_NUM 3 //
#define L4_NUM 4 //
#define L5_FKS 5 //

// defining keys for code layout purposes
#define ________ KC_NO //
#define t_______ KC_TRNS //
#define esc_____ KC_ESC
#define tab_____ KC_TAB
#define del_____ KC_DELETE
#define space___ KC_SPC
#define mute____ KC_MUTE
#define scolon__ KC_SCOLON
#define bslash__ KC_BSLASH
#define capslock KC_CAPSLOCK
#define numlock_ KC_NUMLOCK
#define asterisk KC_KP_ASTERISK
#define KC_LBRCK KC_LBRACKET 
#define KC_RBRCK KC_RBRACKET 
#define __left__ KC_LEFT
#define __right_ KC_RIGHT
#define __win___ KC_LGUI
#define _lshift_ KC_LSHIFT

enum function_id {
    F_NML,
    F_TML,
    F_HMS,
    F_EMS,
    F_SMA,
    F_OMA,
    F_AMN,
    F_QMN,
    F_IMF,
    F_RMG,
    F_UMG,

    F_CTL,
    F_ALT,
    F_SFT,
};

enum macro_id {

    // macros
    MC_EURO,
    MC_POUND,

    MC_AUML,
    MC_OUML,
    MC_UUML,
    MC_SZLIG
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [F_NML] = ACTION_LAYER_TAP_KEY(1, KC_N),            // Momentary Layer1 on N key
    [F_TML] = ACTION_LAYER_TAP_KEY(2, KC_T),            // Momentary Layer2 on T key
    [F_HMS] = ACTION_MODS_TAP_KEY(MOD_LSFT, KC_H),      // Tap for H, Hold for Shift
    [F_EMS] = ACTION_MODS_TAP_KEY(MOD_LSFT, KC_E),      // Tap for E, Hold for Shift
    [F_SMA] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_S),      // Tap for S, Hold for Alt
    [F_QMN] = ACTION_LAYER_TAP_KEY(3, KC_O),            // Tap for Q, Hold for Layer3 (numbers)
    [F_AMN] = ACTION_LAYER_TAP_KEY(4, KC_A),            // Tap for A, Hold for Layer4 (numpad)
    [F_IMF] = ACTION_LAYER_TAP_KEY(5, KC_I),            // Tap for I, Hold for Layer5 (fkeys)
    [F_RMG] = ACTION_MODS_TAP_KEY(MOD_LGUI, KC_R),      // Tap for R, Hold for LGUI (win/command)
    [F_UMG] = ACTION_MODS_TAP_KEY(MOD_LGUI, KC_U),      // Tap for U, Hold for LGUI (win/command)
    [F_OMA] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_O),      // Tap for O, Hold for Alt

    [F_CTL]  = ACTION_MODS_ONESHOT(MOD_LCTL), // oneshot CTRL
    [F_ALT]  = ACTION_MODS_ONESHOT(MOD_LALT), // oneshot ALT
    [F_SFT]  = ACTION_MODS_ONESHOT(MOD_LSFT), // oneshot SHIFT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0:
     * ,-----------------------------------------------.           ,----------------------------------------------------.
     * |  Esc   |   1  |  2  |  3  |  4   |  5  | LGUI |           | PSCR |   6   |   7   |   8  |  9   |   0  |  Del   |
     * |--------+------+-----+-----+------+------------|           |------+-------+-------+------+------+------+--------|
     * |  Tab   |   Q  |  D  |  R  |  W   |  B  | Right|           |  Up  |   J   |   F   |   U  |  P   |      |        |
     * |--------+------+-----+-----+------+-----|      |           |      |-------+-------+------+------+------+--------|
     * |        |   A  |  S  |  H  |  T   |  G  |------|           |------|   Y   |   N   |   E  |  O   |   I  |        |
     * |--------+------+-----+-----+------+-----| Left |           | Down |-------+-------+------+------+------+--------|
     * | LShift |   Z  |  X  |  M  |  C   |  V  |      |           |      |   K   |   L   |  <,  |  >.  |  -_  |        |
     * `--------+------+-----+-----+------+------------'           `--------------+-------+------+------+------+--------'
     *  |  Mute |      |     | LAlt| LCtrl|                                       | RCtrl | RAlt | VOLU | VOLD |NUMLOCK|
     *  `---------------------------------'                                       `------------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |LShift| Home |       | PgUp |LShift|
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      | End  |       |PgDown|      |      |
     *                                 | Tab  | Bkspc|------|       |------|Enter |Space |
     *                                 |      |      | ESC  |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     * 
     *
     * Layer divided in two so we can have transparent keys on the Layer Tap keys
     * Layer 1: Left                                                  Layer2: Right
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |  ^   |  /?  |  Up  |  \|  |  *   |      |           |      |  `~  |  (   |  !   |  )   |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        | Home | Left | Down |Right |  End |------|           |------|  =+  |  [{  |  #   |  ]}  |  '"  |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * | CapsLck|  &   | PgUp |PgDown|  ;:  |  $   |      |           |      |  %   |      |      |      |      |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      | Del  |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     *
     * Numbers layer (normal numbers)
     * Layer 3:
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  7   |  8   |  9   |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      |  4   |  5   |  6   |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  1   |  2   |  3   |      |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |  0   |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     *
     * Numbers layer (NUMPAD)
     * Layer 4:
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  7   |  8   |  9   |  /   |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |------|           |------|      |  4   |  5   |  6   |  *   |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |      |      |      |      |      |           |      |      |  1   |  2   |  3   |  -   |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |      |                                       |  0   |  .   |  ,   |  +   |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     *
     * FKeys layer
     * Layer 5:
     * ,--------------------------------------------------.           ,--------------------------------------------------.
     * |        |  ä   |  ö   |  ü   |  ß   |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
     * |        |      |  F7  |  F8  |  F9  |      |      |           |      |      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |  F11 |  F4  |  F5  |  F6  |  F12 |------|           |------|      |      |      |      |      |        |
     * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
     * |        |      |  F1  |  F2  |  F3  |      |      |           |      |      |      |      |      |      |        |
     * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
     *   |      |      |      |      |  F10 |                                       |      |      |      |      |      |
     *   `----------------------------------'                                       `----------------------------------'
     *                                        ,-------------.       ,-------------.
     *                                        |      |      |       |      |      |
     *                                 ,------|------|------|       |------+------+------.
     *                                 |      |      |      |       |      |      |      |
     *                                 |      |      |------|       |------|      |      |
     *                                 |      |      |      |       |      |      |      |
     *                                 `--------------------'       `--------------------'
     */

[L0_DEF] = KEYMAP(  // Layer0, Left hand.
      esc_____,     KC_1,     KC_2,     KC_3,     KC_4,     KC_5, __win___,
      tab_____, F(F_QMN),     KC_D, F(F_RMG),    KC_W ,     KC_B, __right_,
      ________, F(F_AMN), F(F_SMA), F(F_HMS), F(F_TML),     KC_G,
      F(F_SFT),     KC_Z,     KC_X,     KC_M,     KC_C,     KC_V, __left__,
      mute____, ________, ________, F(F_ALT), F(F_CTL),
                                                        _lshift_,  KC_HOME,
                                                                    KC_END,
                                              tab_____,  KC_BSPC, esc_____,

      // Right hand.
       KC_PSCR,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0, del_____,
         KC_UP,     KC_J,     KC_F, F(F_UMG),     KC_P, ________, ________,
                    KC_Y, F(F_NML), F(F_EMS), F(F_OMA), F(F_IMF), ________,
       KC_DOWN,     KC_K,     KC_L, KC_COMMA,   KC_DOT, KC_MINUS, ________,
                          F(F_CTL), F(F_ALT),  KC_VOLU,  KC_VOLD, numlock_,
       KC_PGUP, _lshift_,
       KC_PGDN,
      ________, KC_ENTER, space___
    ),

[L1_LFT] = KEYMAP(  // Layer1, left hand, to be used with TML
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
       t_______,  KC_CIRC,  KC_SLSH,    KC_UP, bslash__, asterisk, t_______,
       t_______,  KC_HOME, __left__,  KC_DOWN, __right_,   KC_END,
       capslock,  KC_AMPR,  KC_PGUP,  KC_PGDN, scolon__,   KC_DLR, t_______,
       t_______, t_______, t_______, t_______, t_______,
                                                         t_______, t_______,
                                                                   t_______,
                                               t_______, del_____, t_______,

             // right hand empty
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
                 t_______, t_______, t_______, t_______, t_______, t_______,
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
                           t_______, t_______, t_______, t_______, t_______,
       t_______, t_______,
       t_______,
       t_______, t_______,t_______
    ),


[L2_RGT] = KEYMAP(   // Layer2, left hand, empty
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______,
                                                          t_______, t_______,
                                                                    t_______,
                                                t_______, t_______, t_______,

             // right hand, to be used with AML
        KC_F12,   t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, KC_GRAVE,  KC_LPRN, KC_EXLM,   KC_RPRN, t_______, t_______,
                    KC_EQL, KC_LBRCK, KC_HASH,  KC_RBRCK,  KC_QUOT, t_______,
        t_______,  KC_PERC, t_______, t_______, t_______, t_______, t_______,
                            t_______, t_______, t_______, t_______, t_______,
        t_______, t_______,
        t_______,
        t_______, t_______, t_______
    ),

[L3_NUM] = KEYMAP(   // Layer3, numbers
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______,
                                                          t_______, t_______,
                                                                    t_______,
                                                t_______, t_______, t_______,

        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______,     KC_7,     KC_8,     KC_9, t_______, t_______,
                  t_______,     KC_4,     KC_5,     KC_6, t_______, t_______,
        t_______, t_______,     KC_1,     KC_2,     KC_3, t_______, t_______,
                                KC_0, t_______, t_______, t_______, t_______,
        t_______, t_______,
        t_______,
        t_______, t_______, t_______
    ),

[L4_NUM] = KEYMAP(   // Layer3, numbers (numpad)
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______, t_______, t_______,
        t_______, t_______, t_______, t_______, t_______,
                                                          t_______, t_______,
                                                                    t_______,
                                                t_______, t_______, t_______,

             t_______,t_______,t_______,t_______,t_______,t_______,t_______,
             t_______,t_______,  KC_P7,  KC_P8,  KC_P9,KC_PSLS,t_______,
                     t_______,  KC_P4,  KC_P5,  KC_P6,KC_PAST,t_______,
             t_______,t_______,  KC_P1,  KC_P2,  KC_P3,KC_PMNS,t_______,
                       KC_P0,KC_PDOT,KC_PCMM,KC_PPLS,KC_PENT,
        t_______,t_______,
        t_______,
        t_______,t_______,t_______
    ),

[L5_FKS] = KEYMAP(   // Layer4, f-keys
        t_______, M(MC_AUML), M(MC_OUML), M(MC_UUML), M(MC_SZLIG), t_______, t_______,
        t_______,    t_______,      KC_F7,      KC_F8,       KC_F9, t_______, t_______,
        t_______,     KC_F11,      KC_F4,      KC_F5,       KC_F6,  KC_F12,
        t_______,    t_______,      KC_F1,      KC_F2,       KC_F3, t_______, t_______,
        t_______,    t_______,    t_______,    t_______,       KC_F10,
                                   t_______, t_______,
                                            t_______,
                          t_______, t_______, t_______,

             // right hand empty
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
                 t_______, t_______, t_______, t_______, t_______, t_______,
       t_______, t_______, t_______, t_______, t_______, t_______, t_______,
                           t_______, t_______, t_______, t_______, t_______,
       t_______, t_______,
       t_______,
       t_______, t_______,t_______
    )

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {

    // uint8_t hold_shift = keyboard_report->mods & MOD_BIT (KC_LSFT);

    if(!record->event.pressed) {
        unregister_mods(MOD_BIT(KC_LCTL));
        return MACRO_NONE;
    }

	switch (id) {
        case MC_SZLIG: // ß => alt + num 225
            return MACRO( I(10), D(LALT), T(P2), T(P2), T(P5), U(LALT), END );
        case MC_AUML: // ä => alt + num 132
            // if ( hold_shift )  {
            //     return MACRO( I(10), D(LALT), T(P1), T(P4), T(P2), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P3), T(P2), U(LALT), END );
        case MC_OUML: // ö => alt + num 148
            // if ( hold_shift ) {
            //     return MACRO( I(10), D(LALT), T(P1), T(P5), T(P3), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P4), T(P8), U(LALT), END );
        case MC_UUML: // ü => alt + num 129 
            // if ( hold_shift ) {
            //     return MACRO( I(10), D(LALT), T(P1), T(P5), T(P4), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P2), T(P9), U(LALT), END );
	}
	return MACRO_NONE;
}
