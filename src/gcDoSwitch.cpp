// gcDoSwitch.cpp - gcAll_psp.obj
//
// Methods covered:
//   gcDoSwitch::Write(cFile &) const            @ 0x0015033c (148B)
//   gcDoSwitch::~gcDoSwitch(void)               @ 0x00306248 (236B)
//   gcDoSwitch::SetChild(int, gcExpression *)   @ 0x00305e1c (240B)

#include "cBase.h"

class cFile;
class cMemPool;
class cType;
class gcExpression;

extern "C" {
    void gcAction_Write(const void *, cFile &);
    void gcAction___dtor_gcAction_void(void *, int);
    void gcExpressionList_Write(const void *, void *);
}

extern char gcDoSwitchvirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcExpressionList {
public:
    ~gcExpressionList(void);
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DtorDeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void *cMemPool_GetPoolFromPtr(const void *);

class gcDoSwitch {
public:
    static void operator delete(void *);
    void Write(cFile &) const;
    void SetChild(int, gcExpression *);
    ~gcDoSwitch(void);
};

inline void gcDoSwitch::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

// ----------------------------------------------------------------
// gcDoSwitch::Write(cFile &) const @ 0x0015033c (148B)
// ----------------------------------------------------------------
void gcDoSwitch::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    int val = ((int *)this)[3];
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
    gcExpressionList_Write((char *)this + 0x10, &wb);
    gcExpressionList_Write((char *)this + 0x18, &wb);
    wb.End();
}

// Dead branch tail bytes for SetChild — placed in source between Write def
// and SetChild def so SNC flushes them in output after SetChild's body.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fKgcDoSwitchISetChildiP6MgcExpression, 0xF0\n");

// ----------------------------------------------------------------
// gcDoSwitch::SetChild(int, gcExpression *) @ 0x00305e1c (240B)
// ----------------------------------------------------------------
void gcDoSwitch::SetChild(int index, gcExpression *child) {
    (void)index;
    int a = 1;
    int val = ((int *)this)[3];
    int tag = val & 1;
    if (tag != 0) a = 0;

    if (a != 0) {
        int b = 0;
        if (tag != 0) b = 1;
        int newVal;
        if (b != 0) {
            newVal = val & ~1;
            newVal |= 1;
        } else {
            newVal = *(int *)val;
            newVal |= 1;
        }
        val = newVal;
        ((int *)this)[3] = val;
    }

    if (child != (gcExpression *)val) {
        int c = 1;
        int tag2 = val & 1;
        if (tag2 != 0) c = 0;

        if (c != 0) {
            int oldVal = val;
            __asm__ volatile("" : "+r"(oldVal));
            int d = 0;
            if (tag2 != 0) d = 1;
            if (d != 0) {
                val = val & ~1;
                val |= 1;
            } else {
                val = *(int *)val;
                val |= 1;
            }
            ((int *)this)[3] = val;

            if (oldVal != 0) {
                void *vt = *(void **)((char *)oldVal + 4);
                DtorDeleteRecord *rec = (DtorDeleteRecord *)((char *)vt + 0x50);
                short off = rec->offset;
                rec->fn((char *)oldVal + off, (void *)3);
            }
        }

        if (child != 0) {
            ((int *)this)[3] = (int)child;
        }
    }
}

// Dead branch tail bytes for dtor — placed in source between SetChild def
// and dtor def so SNC flushes them in output after dtor's body.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oKgcDoSwitchdtv, 0xEC\n");

// ----------------------------------------------------------------
// gcDoSwitch::~gcDoSwitch(void) @ 0x00306248 (236B)
// ----------------------------------------------------------------
gcDoSwitch::~gcDoSwitch(void) {
    *(void **)((char *)this + 4) = gcDoSwitchvirtualtable;

    ((gcExpressionList *)((char *)this + 0x18))->~gcExpressionList();
    ((gcExpressionList *)((char *)this + 0x10))->~gcExpressionList();

    if ((void *)((char *)this + 0x0C) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
