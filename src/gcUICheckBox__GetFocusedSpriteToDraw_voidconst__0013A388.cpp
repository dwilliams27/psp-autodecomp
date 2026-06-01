// gcUICheckBox::GetFocusedSpriteToDraw(void) const
// Address: 0x0013a388, Size: 112B
// Obj: gcAll_psp.obj
// ODR-WARNING: split-TU local class declaration; do not add methods to include/*.h
//
// gcUICheckBox derives from gcUIControl. Its own focused-sprite handle lives in a
// focus block at this+0x114 (entry+8 holds the handle). Same global entity handle
// table at 0x38890 as gcUIControl::GetFocusedSpriteToDraw: index = (handle & 0xFFFF),
// entry+0x30 holds the owning handle for validation. If no sprite is found for this
// control's own handle, defer to the base-class gcUIControl implementation.

extern void *D_00038890[];

struct gcUISprite;

struct gcUIFocusBlock {
    char pad[8];
    int handle;
};

struct gcUIControl {
    char pad[0x114];
    gcUIFocusBlock focus;
    const gcUISprite *GetFocusedSpriteToDraw(void) const;
};

struct gcUICheckBox : public gcUIControl {
    const gcUISprite *GetFocusedSpriteToDraw(void) const;
};

const gcUISprite *gcUICheckBox::GetFocusedSpriteToDraw(void) const {
    const char *fb = (const char *)&this->focus;
    int handle = *(const int *)(fb + 8);
    int idx = handle & 0xFFFF;
    void *res;
    if (handle == 0) {
        res = 0;
    } else {
        void *cand = D_00038890[idx];
        void *out = 0;
        if (cand != 0) {
            if (*(int *)((char *)cand + 0x30) == handle) {
                out = cand;
            }
        }
        __asm__ volatile("" ::: "memory");
        res = out;
    }
    const gcUISprite *ret = (const gcUISprite *)res;
    if (ret == 0) {
        ret = this->gcUIControl::GetFocusedSpriteToDraw();
    }
    return ret;
}
