// gcUIButton::GetFocusedSpriteToDraw(void) const
// Address: 0x00139c2c, Size: 112B
// Obj: gcAll_psp.obj
// ODR-WARNING: split-TU local class declaration; do not add methods to include/*.h
//
// Mirrors matched sibling gcUICheckBox::GetFocusedSpriteToDraw (0x13a388). The only
// disassembly difference is the focus-block field offset: gcUIButton reads its handle
// at this+0x118 (focus base 0x114, field +4) rather than +0x11C (field +8). Same global
// entity handle table at 0x38890: index = (handle & 0xFFFF), entry+0x30 holds the owning
// handle for validation. If no sprite is found, defer to the base gcUIControl impl.

extern void *D_00038890[];

struct gcUISprite;

struct gcUIFocusBlock {
    char pad[4];
    int handle;
};

struct gcUIControl {
    char pad[0x114];
    gcUIFocusBlock focus;
    const gcUISprite *GetFocusedSpriteToDraw(void) const;
};

struct gcUIButton : public gcUIControl {
    const gcUISprite *GetFocusedSpriteToDraw(void) const;
};

const gcUISprite *gcUIButton::GetFocusedSpriteToDraw(void) const {
    const char *fb = (const char *)&this->focus;
    int handle = *(const int *)(fb + 4);
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
