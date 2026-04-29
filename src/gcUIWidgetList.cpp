#include "gcUIWidgetList.h"

class cMemPool;
class gcEventStackData;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcUIWidgetGroup {
public:
    void Write(cFile &) const;
};

class cTimeValue {
public:
    int mTime;
};

// External helpers for base-class methods (resolved at link time;
// compare_func.py masks the jal relocations so the unresolved target is fine).
void gcUIWidgetGroup_gcUIWidgetGroup(void *self, cBase *parent);
int  gcUIWidgetGroup_IsUpdateEmpty(const void *self, bool a, bool b);
void gcUIWidgetGroup_UpdateChildEffects(void *self, cTimeValue t);
void gcUIWidget_Focus(void *self, bool a, bool b);
void gcUIWidget_CaptureFocus(void *self, bool a);
void gcUIWidget_UpdateUI(void *self, cTimeValue t, const gcEventStackData &d);

// Free-function wrapper declaration so New can call the ctor via an unresolved
// symbol (relocation will be masked during byte comparison).
void gcUIWidgetList_ctor(gcUIWidgetList *self, cBase *parent);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcUIWidgetList::gcUIWidgetList(cBase *) @ 0x00130a3c ──
gcUIWidgetList::gcUIWidgetList(cBase *parent) {
    gcUIWidgetGroup_gcUIWidgetGroup(this, parent);
    *(void **)((char *)this + 4) = (void *)0x38A9A8;
    *(unsigned int *)((char *)this + 0xD8) = (unsigned int)this | 1;
    *(int *)((char *)this + 0xDC) = 0;
    *(short *)((char *)this + 0xE0) = -1;
    *(short *)((char *)this + 0xE2) = -1;
    *(int *)((char *)this + 0xE4) = 0;
}

// ── gcUIWidgetList::IsUpdateEmpty(bool, bool) const @ 0x00131dc0 ──
int gcUIWidgetList::IsUpdateEmpty(bool a, bool b) const {
    int r = 0;
    if (a) {
        r = gcUIWidgetGroup_IsUpdateEmpty(this, a, b);
    }
    return r;
}

// ── gcUIWidgetList::Focus(bool, bool) @ 0x00131d60 ──
void gcUIWidgetList::Focus(bool a, bool b) {
    gcUIWidget_Focus(this, a, b);
    this->FocusCurCell(a);
    gcUIWidget_CaptureFocus(this, a);
    *(unsigned int *)((char *)this + 0xE4) |= 0x40000000;
}

// ── gcUIWidgetList::UpdateUI(cTimeValue, const gcEventStackData &) @ 0x0013185c ──
void gcUIWidgetList::UpdateUI(cTimeValue t, const gcEventStackData &d) {
    this->FillCurVisibleItems();
    gcUIWidget_UpdateUI(this, t, d);
    gcUIWidgetGroup_UpdateChildEffects(this, t);
    this->FocusCurCell(true);
}

// ── gcUIWidgetList::New(cMemPool *, cBase *) @ 0x0027e26c ──
gcUIWidgetList *gcUIWidgetList::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcUIWidgetList *result = 0;
    gcUIWidgetList *obj = (gcUIWidgetList *)entry->fn(base, 0xE8, 4, 0, 0);
    if (obj != 0) {
        gcUIWidgetList_ctor(obj, parent);
        result = obj;
    }
    return result;
}

// ── gcUIWidgetList::Write(cFile &) const @ 0x00130794 ──
void gcUIWidgetList::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const gcUIWidgetGroup *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xE4));
    int val = *(const int *)((const char *)this + 0xD8);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}
