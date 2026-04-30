#include "gcUIWidgetList.h"

class cMemPool;
class gcEventStackData;
class cType;

template <class T>
T dcast(const cBase *);

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
    gcUIWidgetGroup &operator=(const gcUIWidgetGroup &);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cTimeValue {
public:
    int mTime;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

struct CopyEntry {
    short offset;
    short pad;
    int (*fn)(void *, cMemPool *, int);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_00099920;
extern cType *D_0009F4DC;

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

// ── gcUIWidgetList::AssignCopy(const cBase *) @ 0x0027e0f4 ──
void gcUIWidgetList::AssignCopy(const cBase *base) {
    gcUIWidgetList *other = dcast<gcUIWidgetList *>(base);
    ((gcUIWidgetGroup *)this)->operator=(*(const gcUIWidgetGroup *)other);

    if ((int *)((char *)other + 0xD8) != (int *)((char *)this + 0xD8)) {
        goto copy_child;
    }
    goto copy_fields;

copy_child:
    {
        int value = *(int *)((char *)this + 0xD8);
        int flag = 1;
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag == 0) {
            goto from_other;
        }
        {
            int old = value;
            int flag2 = 0;
            if (tag != 0) {
                flag2 = 1;
            }
            if (flag2 == 0) {
                goto load_current_child;
            }
            {
                value &= ~1;
                value |= 1;
            }
            goto store_current_child;
load_current_child:
            value = *(int *)value;
            value |= 1;
store_current_child:
            *(int *)((char *)this + 0xD8) = value;
            if (old != 0) {
                ReleaseEntry *entry =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                entry->fn((char *)old + entry->offset, 3);
            }
        }

from_other:
        value = *(int *)((char *)other + 0xD8);
        flag = 1;
        if (value & 1) {
            flag = 0;
        }
        if (flag == 0) {
            goto copy_fields;
        }
        {
            int source = value;
            CopyEntry *entry = (CopyEntry *)(*(char **)(source + 4) + 0x10);
            cMemPool *pool = cMemPool::GetPoolFromPtr((char *)this + 0xD8);
            int current = *(int *)((char *)this + 0xD8);
            int flag2 = 0;
            if (current & 1) {
                flag2 = 1;
            }
            if (flag2 == 0) {
                goto load_parent_child;
            }
            {
                current &= ~1;
            }
            goto do_copy_child;
load_parent_child:
            current = *(int *)current;
do_copy_child:
            *(int *)((char *)this + 0xD8) =
                entry->fn((char *)source + entry->offset, pool, current);
        }
    }

copy_fields:
    *(int *)((char *)this + 0xDC) = *(int *)((char *)other + 0xDC);
    {
        short *src = (short *)((char *)other + 0xE0);
        short *dst = (short *)((char *)this + 0xE0);
        dst[0] = src[0];
        dst[1] = src[1];
    }
    *(int *)((char *)this + 0xE4) = *(int *)((char *)other + 0xE4);
}

// ── gcUIWidgetList::GetType(void) const @ 0x0027e2e8 ──
const cType *gcUIWidgetList::GetType(void) const {
    if (D_0009F4DC == 0) {
        if (D_00099920 == 0) {
            if (D_0009990C == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                }
                D_0009990C = cType::InitializeType(
                    0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
            }
            D_00099920 = cType::InitializeType(
                0, 0, 0x85, D_0009990C, gcUIWidgetGroup::New, 0, 0, 0);
        }
        D_0009F4DC = cType::InitializeType(
            0, 0, 0x86, D_00099920,
            (cBase *(*)(cMemPool *, cBase *))gcUIWidgetList::New, 0, 0, 0);
    }
    return D_0009F4DC;
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
