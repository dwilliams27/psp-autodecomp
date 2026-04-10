#include "eInputKeyboard.h"
#include "eMovie.h"
#include "gcGameCore.h"
extern "C" int memcmp(const void *, const void *, unsigned int);

extern char D_000454A8[];
extern char D_00045EB8[];
extern char D_00041020[];
extern char D_0037D1EE[];
extern char D_0037D2E4[];
extern char D_0037D2E8[];

typedef eInputKeyboard::eInputState KeyState;
typedef eInput::eButtonState BtnState;

bool eInputKeyboard::PlatformPresent() {
    return false;
}

void eInputKeyboard::RecheckPresent() {
    *(char *)(D_000454A8 + 0xA09) = 1;
}

void eInputKeyboard::Reset() {
    ((KeyState *)D_000454A8)->Reset();
    ((KeyState *)D_00045EB8)->Reset();
}

bool eInputKeyboard::Initialize() {
    if (PlatformInitialize()) {
        return true;
    }
    return false;
}

void eInputKeyboard::Update(cTimeValue dt, bool reset, bool accumTime) {
    KeyState *state = (KeyState *)D_000454A8;
    KeyState backup;
    KeyState *backupBase = &backup;
    char *asciiTable = D_0037D1EE;

    for (int i = 0; i < 107; i++) {
        backup.buttons[i] = state->buttons[i];
    }
    backup.asciiKey = state->asciiKey;
    backup.present = state->present;
    backup.changed = state->changed;
    backup.timeAccum = state->timeAccum;

    PlatformPreUpdate();

    if (reset) {
        for (int i = 0; i < 107; i++) {
            state->buttons[i].rawValue = 0;
        }
        state->present = PlatformPresent();
        state->changed = (backup.present != state->present) ? 1 : 0;
        PlatformUpdate();
    } else {
        for (int i = 0; i < 107; i++) {
            state->buttons[i].rawInt = *(int *)D_00041020;
        }
    }

    unsigned char up = state->buttons[70].rawValue;
    unsigned char down = state->buttons[71].rawValue;
    unsigned char left = state->buttons[63].rawValue;
    unsigned char right = state->buttons[1].rawValue;
    unsigned char numLock = state->buttons[88].rawValue;

    if (up > 0 || state->buttons[74].rawValue > 0) {
        state->buttons[68].rawValue = 0xFF;
    } else {
        state->buttons[68].rawValue = 0;
    }

    if (down > 0 || state->buttons[73].rawValue > 0) {
        state->buttons[69].rawValue = 0xFF;
    } else {
        state->buttons[69].rawValue = 0;
    }

    if (left > 0 || state->buttons[67].rawValue > 0) {
        state->buttons[62].rawValue = 0xFF;
    } else {
        state->buttons[62].rawValue = 0;
    }

    if (right > 0 || state->buttons[61].rawValue > 0 || state->buttons[94].rawValue > 0 || state->buttons[72].rawValue > 0) {
        state->buttons[105].rawValue = 0xFF;
    } else {
        state->buttons[105].rawValue = 0;
    }

    if (right > 0 || state->buttons[61].rawValue > 0 || state->buttons[94].rawValue > 0 || state->buttons[72].rawValue > 0) {
        state->buttons[106].rawValue = 0xFF;
    } else {
        state->buttons[106].rawValue = 0;
    }

    if (numLock == 0) {
        state->buttons[78].rawValue = (state->buttons[78].rawValue > 0 || state->buttons[95].rawValue > 0) ? 0xFF : 0;
        state->buttons[79].rawValue = (state->buttons[79].rawValue > 0 || state->buttons[102].rawValue > 0) ? 0xFF : 0;
        state->buttons[80].rawValue = (state->buttons[80].rawValue > 0 || state->buttons[104].rawValue > 0) ? 0xFF : 0;
        state->buttons[81].rawValue = (state->buttons[81].rawValue > 0 || state->buttons[92].rawValue > 0) ? 0xFF : 0;
        state->buttons[82].rawValue = (state->buttons[82].rawValue > 0 || state->buttons[96].rawValue > 0) ? 0xFF : 0;
        state->buttons[83].rawValue = (state->buttons[83].rawValue > 0 || state->buttons[98].rawValue > 0) ? 0xFF : 0;
        state->buttons[84].rawValue = (state->buttons[84].rawValue > 0 || state->buttons[103].rawValue > 0) ? 0xFF : 0;
        state->buttons[85].rawValue = (state->buttons[85].rawValue > 0 || state->buttons[99].rawValue > 0) ? 0xFF : 0;
        state->buttons[86].rawValue = (state->buttons[86].rawValue > 0 || state->buttons[97].rawValue > 0) ? 0xFF : 0;
        state->buttons[87].rawValue = (state->buttons[87].rawValue > 0 || state->buttons[101].rawValue > 0) ? 0xFF : 0;
    }

    state->asciiKey = 0;

    for (int i = 0; i < 107; i++) {
        BtnState *btn = &state->buttons[i];
        btn->Update(backupBase->buttons[i], true, false, 0);
        if (btn->justPressed || btn->justReleased) {
            if (*asciiTable) {
                state->asciiKey = KeyToASCII((eKey)i);
            }
        }
        asciiTable += 2;
    }

    if (memcmp(backupBase, state, 0xA08) != 0) {
        state->timeAccum = 0;
    } else if (accumTime) {
        state->timeAccum += *(int *)&dt;
    }
}

void eMovie::SetDrawCallback(void (*callback)(void *), void *data) {
    *(void (**)(void *))(D_0037D2E4) = callback;
    *(void **)(D_0037D2E8) = data;
}
