// gcUIWidgetGroup::InitialUpdateUI(void) @ 0x000DDE40
// Split-TU: local declarations to match SNC codegen for this object.
//
// Mirrors gcUIWidget::InitialUpdateUI: calls the base-class InitialUpdateUI()
// first, then runs an IsUpdateEmpty-style probe through a delegate stored in a
// member object at this+4 (adjustor short + raw function pointer at +0x108),
// and folds the boolean result into mFlags at this+0xB0.

class gcUIWidget {
public:
    char pad_base[0x28];
    void InitialUpdateUI(void);
};

// Delegate record: a this-adjustor and a plain function pointer.
struct gcProbeDelegate {
    short adjustor;     // +0x0
    int (*fn)(void *, bool, bool);  // +0x4
};

class gcUIWidgetGroup : public gcUIWidget {
public:
    char pad_grp[0xB0 - 0x28];
    unsigned int mGroupFlags;   // 0xB0
    void InitialUpdateUI(void);
};

void gcUIWidgetGroup::InitialUpdateUI(void) {
    gcUIWidget::InitialUpdateUI();

    char *holder = *(char *const *)((char *)this + 0x4);
    gcProbeDelegate *d = (gcProbeDelegate *)(holder + 0x108);
    void *recv = (char *)this + d->adjustor;
    int result = d->fn(recv, true, false);
    int masked = result & 0xFF;

    if (masked & 0xFF) {
        mGroupFlags |= 0x40000000;
    } else {
        mGroupFlags &= 0xBFFFFFFF;
    }
}
