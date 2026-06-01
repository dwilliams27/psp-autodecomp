// gcProfile::GetControllerId(void) const @ 0x0010061C
// Looks up `this` in gcGameSettings::Get()->mProfiles array (length at [-4]),
// then if the found index is in [0,8) indexes a global controller table
// (*0x37D87C) by index*0x44 and returns its field at +0x20. Out of range
// returns *(NULL+0x20) (a deliberate deref of the computed pointer).

class gcProfile;

struct gcControllerEntry {
    char _pad[0x20];
    int mId; // 0x20
};

struct gcGameSettings {
    char _pad54[0x54];
    const gcProfile **mProfiles; // 0x54
    static gcGameSettings *Get(void);
};

extern gcControllerEntry *D_0037D87C;

class gcProfile {
public:
    int GetControllerId(void) const;
};

int gcProfile::GetControllerId(void) const {
    const gcProfile **arr = gcGameSettings::Get()->mProfiles;

    int i = 0;
    int off = 0;
    int idx;
    for (;;) {
        int count = 0;
        if (arr) {
            count = ((int *)arr)[-1];
        }
        if (i < count) {
            if (this == *(const gcProfile **)((char *)arr + off)) {
                idx = i;
                break;
            }
            i += 1;
            off += 4;
            continue;
        }
        idx = -1;
        break;
    }

    gcControllerEntry *e = 0;
    if (idx >= 0 && idx < 8) {
        e = (gcControllerEntry *)((char *)D_0037D87C + idx * 0x44);
    }
    return e->mId;
}
