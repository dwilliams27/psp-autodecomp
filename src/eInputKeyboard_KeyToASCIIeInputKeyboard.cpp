#include "eInputKeyboard.h"

extern char D_000454A8[];
extern char D_0037D1EE[];

typedef eInputKeyboard::eInputState KeyState;

char eInputKeyboard::KeyToASCII(eKey key) {
    KeyState *state = (KeyState *)D_000454A8;

    unsigned char caps_raw = state->buttons[62].rawValue;
    unsigned char numlock_raw = state->buttons[88].rawValue;
    unsigned char shift_raw = state->buttons[58].rawValue;
    int caps = caps_raw != 0;
    int numlock = numlock_raw != 0;
    int shift = shift_raw != 0;

    int shifted = caps != 0;

    if (key >= (eKey)24 && key < (eKey)50) {
        shifted = (shift ^ caps) != 0;
    }

    if (key >= (eKey)88 && key < (eKey)105) {
        if (!numlock) return 0;
    }

    return D_0037D1EE[key * 2 + shifted];
}
