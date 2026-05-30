#include "eInputJoystick.h"

// Free function: ePlatformInput_AnyButtonPressed(eInputJoystick::eInputState *)
// Mangled: __0FfePlatformInput_AnyButtonPressedP65OeInputJoystickLeInputState

struct ButtonEntry {
    char pad[4];
    unsigned char pressed;
    char pad2[19];
};

int ePlatformInput_AnyButtonPressed(eInputJoystick::eInputState *state) {
    int i = 4;
    ButtonEntry *entry = (ButtonEntry *)((char *)state + 0x60);
    do {
        if ((int)entry->pressed > 0) {
            return 0xFF;
        }
        i++;
        entry++;
    } while (i < 41);
    return 0;
}
