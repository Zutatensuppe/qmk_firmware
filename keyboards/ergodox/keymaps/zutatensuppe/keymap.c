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
#define ______ KC_NO //
#define _____t KC_TRNS //
#define esc___ KC_ESC
#define tab___ KC_TAB
#define enter_ KC_ENTER
#define del___ KC_DELETE
#define bspc__ KC_BSPC
#define space_ KC_SPC
#define dollar KC_DLR
#define scolon KC_SCOLON
#define bslash KC_BSLASH
#define capslo KC_CAPSLOCK
#define numlo_ KC_NUMLOCK
#define aster_ KC_KP_ASTERISK
#define amp___ KC_AMPR
#define lbrck_ KC_LBRACKET
#define rbrck_ KC_RBRACKET
#define lprn__ KC_LPRN
#define rprn__ KC_RPRN
#define up____ KC_UP
#define down__ KC_DOWN
#define left__ KC_LEFT
#define right_ KC_RIGHT
#define pgup__ KC_PGUP
#define pgdn__ KC_PGDN
#define home__ KC_HOME
#define end___ KC_END
#define win___ KC_LGUI
#define print_ KC_PSCR
#define lshft_ KC_LSHIFT
#define dot___ KC_DOT
#define comma_ KC_COMMA
#define equls_ KC_EQL
#define minus_ KC_MINUS
#define mute__ KC_MUTE
#define volup_ KC_VOLU
#define voldn_ KC_VOLD
#define pad_0_ KC_P0
#define pad_1_ KC_P1
#define pad_2_ KC_P2
#define pad_3_ KC_P3
#define pad_4_ KC_P4
#define pad_5_ KC_P5
#define pad_6_ KC_P6
#define pad_7_ KC_P7
#define pad_8_ KC_P8
#define pad_9_ KC_P9
#define p_dot_ KC_PDOT
#define p_com_ KC_PCMM
#define p_sub_ KC_PMNS
#define p_add_ KC_PPLS 
#define p_mul_ KC_PAST
#define p_div_ KC_PSLS
#define quot__ KC_QUOT
#define perc__ KC_PERC
#define f1____ KC_F1
#define f2____ KC_F2
#define f3____ KC_F3
#define f4____ KC_F4
#define f5____ KC_F5
#define f6____ KC_F6
#define f7____ KC_F7
#define f8____ KC_F8
#define f9____ KC_F9
#define f10___ KC_F10
#define f11___ KC_F11
#define f12___ KC_F12
#define hash__ KC_HASH
#define excl__ KC_EXLM
#define btick_ KC_GRAVE
#define circ__ KC_CIRC
#define slash_ KC_SLSH

#define KEYMAP_EASY(                                            \
                                                                \
    /* left hand */                /* right hand */             \
    k00,k01,k02,k03,k04,k05,k06,   k07,k08,k09,k0A,k0B,k0C,k0D, \
    k10,k11,k12,k13,k14,k15,k16,   k17,k18,k19,k1A,k1B,k1C,k1D, \
    k20,k21,k22,k23,k24,k25,           k28,k29,k2A,k2B,k2C,k2D, \
    k30,k31,k32,k33,k34,k35,k36,   k37,k38,k39,k3A,k3B,k3C,k3D, \
    k40,k41,k42,k43,k44,                   k49,k4A,k4B,k4C,k4D, \
                        k55,k56,   k57,k58,                     \
                            k54,   k59,                         \
                    k53,k52,k51,   k5C,k5B,k5A                  \
    )                                                           \
                                                                \
   /* matrix positions */                                       \
   {                                                            \
    { k00, k10, k20, k30, k40, KC_NO },   \
    { k01, k11, k21, k31, k41, k51 },   \
    { k02, k12, k22, k32, k42, k52 },   \
    { k03, k13, k23, k33, k43, k53 },   \
    { k04, k14, k24, k34, k44, k54 },   \
    { k05, k15, k25, k35, KC_NO, k55 },   \
    { k06, k16, KC_NO, k36, KC_NO, k56 },   \
                                                                 \
    { k07, k17, KC_NO, k37,KC_NO, k57 },   \
    { k08, k18, k28, k38,KC_NO, k58 },   \
    { k09, k19, k29, k39, k49, k59 },   \
    { k0A, k1A, k2A, k3A, k4A, k5A },   \
    { k0B, k1B, k2B, k3B, k4B, k5B },   \
    { k0C, k1C, k2C, k3C, k4C, k5C },   \
    { k0D, k1D, k2D, k3D, k4D, KC_NO }    \
   }

enum function_id {
    fNL,
    fTL,
    fHS,
    fES,
    fSA,
    fOA,
    fAN,
    fQN,
    fIF,
    fRG,
    fUG,

    f_C,
    f_A,
    f_S,
};

enum macro_id {
    mAU,
    mOU,
    mUM,
    mSZ
};

/*
 * Fn action definition
 */
const uint16_t PROGMEM fn_actions[] = {
    [fNL] = ACTION_LAYER_TAP_KEY(1, KC_N),            // Momentary Layer1 on N key
    [fTL] = ACTION_LAYER_TAP_KEY(2, KC_T),            // Momentary Layer2 on T key
    [fHS] = ACTION_MODS_TAP_KEY(MOD_LSFT, KC_H),      // Tap for H, Hold for Shift
    [fES] = ACTION_MODS_TAP_KEY(MOD_LSFT, KC_E),      // Tap for E, Hold for Shift
    [fSA] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_S),      // Tap for S, Hold for Alt
    [fAN] = ACTION_LAYER_TAP_KEY(3, KC_A),            // Tap for A, Hold for Layer3 (numbers)
    [fQN] = ACTION_LAYER_TAP_KEY(4, KC_Q),            // Tap for Q, Hold for Layer4 (numpad)
    [fIF] = ACTION_LAYER_TAP_KEY(5, KC_I),            // Tap for I, Hold for Layer5 (fkeys)
    [fRG] = ACTION_MODS_TAP_KEY(MOD_LGUI, KC_R),      // Tap for R, Hold for LGUI (win/command)
    [fUG] = ACTION_MODS_TAP_KEY(MOD_LGUI, KC_U),      // Tap for U, Hold for LGUI (win/command)
    [fOA] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_O),      // Tap for O, Hold for Alt

    [f_C]  = ACTION_MODS_ONESHOT(MOD_LCTL), // oneshot CTRL
    [f_A]  = ACTION_MODS_ONESHOT(MOD_LALT), // oneshot ALT
    [f_S]  = ACTION_MODS_ONESHOT(MOD_LSFT), // oneshot SHIFT
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

[L0_DEF] = KEYMAP_EASY( // Layer0, Left hand.
esc___,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5, win___,    print_,   KC_6,   KC_7,   KC_8,   KC_9,   KC_0, del___,
tab___, F(fQN),   KC_D, F(fRG),  KC_W ,   KC_B, right_,    up____,   KC_J,   KC_F, F(fUG),   KC_P, ______, ______,
______, F(fAN), F(fSA), F(fHS), F(fTL),   KC_G,                      KC_Y, F(fNL), F(fES), F(fOA), F(fIF), ______,
F(f_S),   KC_Z,   KC_X,   KC_M,   KC_C,   KC_V, left__,    down__,   KC_K,   KC_L, comma_, dot___, minus_, ______,
mute__, ______, ______, F(f_A), F(f_C),                                    F(f_C), F(f_A), volup_, voldn_, numlo_,
                                        lshft_, home__,    pgup__, lshft_,
                                                end___,    pgdn__,
                                tab___, bspc__, esc___,    ______, enter_, space_
),

[L1_LFT] = KEYMAP_EASY( // Layer1, left hand, to be used with TML
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, circ__, slash_, up____, bslash, aster_, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, home__, left__, down__, right_, end___,                    _____t, _____t, _____t, _____t, _____t, _____t,
capslo, amp___, pgup__, pgdn__, scolon, dollar, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, _____t,                                    _____t, _____t, _____t, _____t, _____t,
                                        _____t, _____t,    _____t, _____t,
                                                _____t,    _____t,
                                _____t, del___, _____t,    _____t, _____t, _____t
),


[L2_RGT] = KEYMAP_EASY( // Layer2, left hand, empty
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, btick_, lprn__, excl__, rprn__, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t,                    equls_, lbrck_, hash__, rbrck_, quot__, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, perc__, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, _____t,                                    _____t, _____t, _____t, _____t, _____t,
                                        _____t, _____t,    _____t, _____t,
                                                _____t,    _____t,
                                _____t, _____t, _____t,    _____t, _____t, _____t
),

[L3_NUM] = KEYMAP_EASY( // Layer3, numbers
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t,   KC_7,   KC_8,   KC_9, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t,                    _____t,   KC_4,   KC_5,   KC_6, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t,   KC_1,   KC_2,   KC_3, _____t, _____t,
_____t, _____t, _____t, _____t, _____t,                                      KC_0, _____t, _____t, _____t, _____t,
                                        _____t, _____t,    _____t, _____t,
                                                _____t,    _____t,
                                _____t, _____t, _____t,    _____t, _____t, _____t
),

[L4_NUM] = KEYMAP_EASY( // Layer3, numbers (numpad)
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, pad_7_, pad_8_, pad_9_, p_div_, _____t,
_____t, _____t, _____t, _____t, _____t, _____t,                    _____t, pad_4_, pad_5_, pad_6_, p_mul_, _____t,
_____t, _____t, _____t, _____t, _____t, _____t, _____t,    _____t, _____t, pad_1_, pad_2_, pad_3_, p_sub_, _____t,
_____t, _____t, _____t, _____t, _____t,                                    pad_0_, p_dot_, p_com_, p_add_, _____t,
                                        _____t, _____t,    _____t, _____t,
                                                _____t,    _____t,
                                _____t, _____t, _____t,    _____t, _____t, _____t
),

[L5_FKS] = KEYMAP_EASY( // Layer4, f-keys
_____t, M(mAU), M(mOU), M(mUM), M(mSZ), _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, f7____, f8____, f9____, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, f11___, f4____, f5____, f6____, f12___,                    _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, f1____, f2____, f3____, _____t, _____t,    _____t, _____t, _____t, _____t, _____t, _____t, _____t,
_____t, _____t, _____t, _____t, f10___,                                    _____t, _____t, _____t, _____t, _____t,
                                        _____t, _____t,    _____t, _____t,
                                                _____t,    _____t,
                                _____t, _____t, _____t,    _____t, _____t,_____t
)

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {

    // uint8_t hold_shift = keyboard_report->mods & MOD_BIT (KC_LSFT);

    if(!record->event.pressed) {
        unregister_mods(MOD_BIT(KC_LCTL));
        return MACRO_NONE;
    }

	switch (id) {
        case mSZ: // ß => alt + num 225
            return MACRO( I(10), D(LALT), T(P2), T(P2), T(P5), U(LALT), END );
        case mAU: // ä => alt + num 132
            // if ( hold_shift )  {
            //     return MACRO( I(10), D(LALT), T(P1), T(P4), T(P2), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P3), T(P2), U(LALT), END );
        case mOU: // ö => alt + num 148
            // if ( hold_shift ) {
            //     return MACRO( I(10), D(LALT), T(P1), T(P5), T(P3), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P4), T(P8), U(LALT), END );
        case mUM: // ü => alt + num 129 
            // if ( hold_shift ) {
            //     return MACRO( I(10), D(LALT), T(P1), T(P5), T(P4), U(LALT), END );
            // }
            return MACRO( I(10), D(LALT), T(P1), T(P2), T(P9), U(LALT), END );
	}
	return MACRO_NONE;
}
