// gcDoLobbyFriendOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e09e4 gcDoLobbyFriendOp::New(cMemPool *, cBase *) static  156B
//   0x002e0b98 gcDoLobbyFriendOp::Write(cFile &) const             112B

#include "cBase.h"

class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class gcDoLobbyFriendOp {
public:
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    ~gcDoLobbyFriendOp(void);
    void Write(cFile &) const;
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct CopySlot {
    short offset;
    short pad;
    int (*fn)(void *, cMemPool *, int);
};

void gcAction_gcAction(gcDoLobbyFriendOp *, cBase *);
void gcAction_Write(const gcDoLobbyFriendOp *, cFile &);
void *cMemPool_GetPoolFromPtr(const void *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern char gcDoLobbyFriendOpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyFriendOp asm("D_0009F690");

inline void gcDoLobbyFriendOp::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

// ── gcDoLobbyFriendOp::AssignCopy(const cBase *) @ 0x002e0710 ──
void gcDoLobbyFriendOp::AssignCopy(const cBase *base) {
    const gcDoLobbyFriendOp *other = 0;
    char *dstDesired = (char *)this + 0x10;

    if (base != 0) {
        if (!type_gcDoLobbyFriendOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894,
                            (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLobbyFriendOp = cType::InitializeType(
                0, 0, 0x1C6, type_action, gcDoLobbyFriendOp::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = type_gcDoLobbyFriendOp;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *type = entry->fn((void *)((const char *)base + offset));
        int ok;

        if (target != 0) {
            goto have_target;
        }
        ok = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop:
            if (type == target) {
                ok = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto invalid;
            }
        } else {
invalid:
            ok = 0;
        }

cast_done:
        if (ok != 0) {
            other = (const gcDoLobbyFriendOp *)base;
        }
    }

    int flags = *(int *)((char *)this + 8) & -4;
    *(int *)((char *)this + 8) = flags;
    flags |= *(const int *)((const char *)other + 8) & 3;
    *(int *)((char *)this + 8) = flags;

    *(int *)((char *)this + 0x0C) =
        *(const int *)((const char *)other + 0x0C);

    if ((const char *)other + 0x10 != dstDesired) {
        int oldValue = *(int *)((char *)this + 0x10);
        int oldOwned = 1;
        int oldTagged = oldValue & 1;
        if (oldTagged != 0) {
            oldOwned = 0;
        }
        if (oldOwned != 0) {
            int untag = 0;
            if (oldTagged != 0) {
                untag = 1;
            }
            int replacement;
            if (untag != 0) {
                replacement = oldValue & -2;
            } else {
                replacement = *(int *)oldValue;
            }
            replacement |= 1;
            if (oldValue != 0) {
                *(int *)((char *)this + 0x10) = replacement;
                void *typeInfo = *(void **)(oldValue + 4);
                DtorSlot *slot = (DtorSlot *)((char *)typeInfo + 0x50);
                short off = slot->offset;
                void (*fn)(void *, int) = slot->fn;
                fn((char *)oldValue + off, 3);
            } else {
                *(int *)((char *)this + 0x10) = replacement;
            }
        }

        int srcValue = *(const int *)((const char *)other + 0x10);
        int srcOwned = 1;
        int srcTagged = srcValue & 1;
        if (srcTagged != 0) {
            srcOwned = 0;
        }
        if (srcOwned != 0) {
            void *typeInfo = *(void **)(srcValue + 4);
            CopySlot *slot = (CopySlot *)((char *)typeInfo + 0x10);
            short off = slot->offset;
            char *srcObject = (char *)srcValue + off;
            cMemPool *pool = cMemPool::GetPoolFromPtr(dstDesired);

            int dstValue = *(int *)((char *)this + 0x10);
            int dstTagged = dstValue & 1;
            int current = 0;
            if (dstTagged != 0) {
                current = 1;
            }
            if (current != 0) {
                dstValue &= -2;
            } else {
                dstValue = *(int *)dstValue;
            }

            *(int *)((char *)this + 0x10) = slot->fn(srcObject, pool, dstValue);
        }
    }

    *(unsigned char *)((char *)this + 0x14) =
        *(const unsigned char *)((const char *)other + 0x14);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── gcDoLobbyFriendOp::New(cMemPool *, cBase *) @ 0x002e09e4 ──
cBase *gcDoLobbyFriendOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyFriendOp *result = 0;
    gcDoLobbyFriendOp *obj = (gcDoLobbyFriendOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyFriendOpvirtualtable;
        ((int *)obj)[3] = 2;
        ((int *)obj)[4] = (int)obj | 1;
        *((char *)obj + 20) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoLobbyFriendOp::GetType(void) const @ 0x002e0a80 ──
const cType *gcDoLobbyFriendOp::GetType(void) const {
    if (!type_gcDoLobbyFriendOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyFriendOp = cType::InitializeType(
            0, 0, 0x1C6, type_action, gcDoLobbyFriendOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyFriendOp;
}

// ── gcDoLobbyFriendOp::Write(cFile &) @ 0x002e0b98 ──
void gcDoLobbyFriendOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x14));
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── gcDoLobbyFriendOp::~gcDoLobbyFriendOp(void) @ 0x002e1324 ──
gcDoLobbyFriendOp::~gcDoLobbyFriendOp(void) {
    *(void **)((char *)this + 4) = gcDoLobbyFriendOpvirtualtable;

    if ((void *)((char *)this + 0x10) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x10) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}
