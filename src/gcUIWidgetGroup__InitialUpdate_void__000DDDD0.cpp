// gcUIWidgetGroup::InitialUpdate(void)
// Address: 0x000dddd0, Size: 112B
// Obj: gcAll_psp.obj
//
// Pattern: chain to base gcUIWidget::InitialUpdate(), then dispatch through a
// data-resident callback descriptor stored in the object referenced by m4 (at
// offset 0x108). The descriptor holds a half-word `this`-adjustment followed by
// a function pointer. The callback is invoked on (this + adjust) with args
// (0, 0); the byte result drives bit 31 of the group flags word at 0xB0.

class gcUIWidgetGroup;

// Base supplies the chained InitialUpdate() called first.
class gcUIWidget {
public:
    char pad_00[0x04];
    void InitialUpdate(void);
};

typedef int (*UpdateFn)(void *self, int, int);

// Callback descriptor: 2-byte adjustment then the function pointer.
struct gcUIWidgetCallback {
    short mAdjust;     // +0x00 -> at object offset 0x108
    UpdateFn mFn;      // +0x04 -> at object offset 0x10C
};

class gcUIWidgetGroupHelper {
public:
    char pad_00[0x108];
    gcUIWidgetCallback mCallback;    // 0x108
};

class gcUIWidgetGroup : public gcUIWidget {
public:
    char pad_04[0xAC];               // up to 0xB0
    unsigned int mFlags2;            // 0xB0
    void InitialUpdate(void);
};

void gcUIWidgetGroup::InitialUpdate(void) {
    gcUIWidget::InitialUpdate();

    gcUIWidgetGroupHelper *helper = *(gcUIWidgetGroupHelper **)((char *)this + 4);
    gcUIWidgetCallback *cb = &helper->mCallback;
    int result = cb->mFn((char *)this + cb->mAdjust, 0, 0);
    int masked = result & 0xFF;
    if (masked & 0xFF) {
        mFlags2 |= 0x80000000;
    } else {
        mFlags2 &= 0x7FFFFFFF;
    }
}
