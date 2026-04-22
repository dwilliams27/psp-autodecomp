#include "gcUIDialog.h"
#include "gcUI.h"
#include "gcUIWidgetGroup.h"

class cMemBlockAllocation {
public:
    cMemBlockAllocation(void *, bool);
    ~cMemBlockAllocation();
    char _pad[0x18];
};

class cNamed {
public:
    void GenerateName(const char *);
};

template <class T>
class cSubHandleT {
public:
    int mValue;
    cSubHandleT();
    cSubHandleT(const cSubHandleT &);
    ~cSubHandleT();
};

void gcUIDialog::CalcInstanceSize(void) {
}

void gcUIDialog::OnFinalOpen(void) {
    mFlags = mFlags & ~0x10;
}

void gcUIDialog::ClearEvents(void) {
    mEvent0 = 0;
    mEvent1 = 0;
    mEvent2 = 0;
    mEvent3 = 0;
    mEvent4 = 0;
}

void gcUIDialog::Reset(cMemPool *pool, bool flag) {
    ((gcUIWidgetGroup *)((char *)this + 0x98))->Reset(pool, true);
}

void gcUIDialog::Focus(void) {
    if (mpUI) {
        mpUI->ActualOpenDialog(this);
    }
}

void gcUIDialog::Close(void) {
    if (mpUI) {
        mpUI->CloseDialog(this, 0, 0);
    }
}

void gcUIDialog::OnFinalClose(void) {
    mpUI->ActualCloseDialog(this);
    *(int *)((char *)this + 0x54) = *(int *)((char *)this + 0x54) & ~0x10;
}

struct gcUIDialog_Slot {
    int pad0;
    gcUIWidget *widget;
    int pad8;
    int padC;
    int pad10;
};

gcUIWidget *gcUIDialog::GetControl(cSubHandleT<gcUIWidget> handle, int arg2) const {
    int h = handle.mValue;
    if (h == 0) return 0;
    int idx = (h & 0xFFFF) + arg2;
    if (idx >= *(int *)((char *)this + 0x90)) return 0;
    gcUIWidget *widget = (*(gcUIWidget ***)((char *)this + 0x94))[idx];
    gcUIWidget *result = 0;
    if (widget != 0) {
        int v = *(volatile int *)&handle.mValue;
        int eq = v == *(int *)((char *)widget + 0x28);
        if ((eq & 0xff) | arg2) result = widget;
    }
    return result;
}

void gcUIDialog::OnSpawned(cMemPool *pool, const cName &name) {
    int *vt = (int *)(((char **)this)[1] + 0x38);
    short adj = *(short *)vt;
    ((void (*)(char *, cMemPool *, int))vt[1])((char *)this + adj, pool, 1);
    ((cNamed *)this)->GenerateName((const char *)&name);
    *(unsigned short *)((char *)this + 0x28) |= 0x10;
    *(int *)((char *)this + 0x2C) = *(int *)((char *)this + 0x1E4);
}

void gcUIDialog::FreeDynamicInstance(gcUIDialog *dialog) {
    cMemBlockAllocation _alloc(dialog, true);
    if (dialog != 0) {
        int *vt = (int *)(((char **)dialog)[1] + 0x50);
        short thunk = *(short *)vt;
        ((void (*)(char *, int))vt[1])((char *)dialog + thunk, 3);
    }
}

int gcUIDialog::IsNextMouseOver(gcUIWidget *widget, int skip) const {
    int i;
    gcUIWidget *focus;
    if (skip >= 0) {
        focus = *(gcUIWidget **)((char *)this + 0x178);
        if (widget == focus) return 1;
    }
    i = 0;
    gcUIDialog_Slot *p = (gcUIDialog_Slot *)((char *)this + 0x18C);
    for (; i < 4; i++) {
        if (i != skip) {
            if (p->widget == widget) return 1;
        }
        p++;
    }
    return 0;
}
