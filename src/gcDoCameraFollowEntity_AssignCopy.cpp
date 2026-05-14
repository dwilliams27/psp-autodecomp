// gcDoCameraFollowEntity_AssignCopy.cpp - split TU for AssignCopy.
// ODR-WARNING: this TU re-declares gcDoCameraFollowEntity locally with only the
// AssignCopy method; the rest of the class lives in src/gcDoCameraFollowEntity.cpp
// (New/GetType/Write/GetText) and src/gcDoCameraFollowEntity_Read.cpp /
// src/gcDoCameraFollowEntity_dtor.cpp.

#include "cBase.h"

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

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct DtorDeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct CopyOpRec {
    short offset;
    short _pad;
    int (*fn)(void *, cMemPool *, int);
};

class gcDesiredValue {
public:
    int mValue;
    gcDesiredValue &operator=(const gcDesiredValue &other);
};

inline gcDesiredValue &gcDesiredValue::operator=(const gcDesiredValue &other) {
    if (&other != this) {
        int oldValue = mValue;
        int release = 1;
        int oldTagged = oldValue & 1;
        if (oldTagged != 0) {
            release = 0;
        }
        if (release != 0) {
            int releaseTagged = 0;
            if (oldTagged != 0) {
                releaseTagged = 1;
            }
            int stored;
            if (releaseTagged != 0) {
                stored = oldValue & -2;
            } else {
                stored = *(int *)oldValue;
            }
            mValue = stored | 1;
            if (oldValue != 0) {
                void *classDesc = *(void **)(oldValue + 4);
                DtorDeleteRecord *entry =
                    (DtorDeleteRecord *)((char *)classDesc + 0x50);
                short offset = entry->offset;
                void (*fn)(void *, void *) = entry->fn;
                fn((char *)oldValue + offset, (void *)3);
            }
        }
        int sourceValue = other.mValue;
        int copy = 1;
        if ((sourceValue & 1) != 0) {
            copy = 0;
        }
        if (copy != 0) {
            void *classDesc = *(void **)(sourceValue + 4);
            CopyOpRec *entry = (CopyOpRec *)((char *)classDesc + 0x10);
            short offset = entry->offset;
            void *sourceObject = (char *)sourceValue + offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(this);
            int currentValue = mValue;
            int currentTagged = 0;
            if ((currentValue & 1) != 0) {
                currentTagged = 1;
            }
            int currentObject;
            if (currentTagged != 0) {
                currentObject = currentValue & -2;
            } else {
                currentObject = *(int *)currentValue;
            }
            int (*fn)(void *, cMemPool *, int) = entry->fn;
            mValue = fn(sourceObject, pool, currentObject);
        }
    }
    return *this;
}

class gcDoCameraFollowEntity : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
};

extern "C" void gcDesiredEntity_op_eq(void *, const void *)
    __asm__("__0oPgcDesiredEntityasRC6PgcDesiredEntity");

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoCameraFollow;
static cType *type_gcDoCameraFollowEntity;

void gcDoCameraFollowEntity::AssignCopy(const cBase *base) {
    const gcDoCameraFollowEntity *other = 0;

    if (base != 0) {
        if (!type_gcDoCameraFollowEntity) {
            if (!type_gcDoCameraFollow) {
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
                type_gcDoCameraFollow = cType::InitializeType(
                    0, 0, 0x24F, type_action, 0, 0, 0, 0);
            }
            type_gcDoCameraFollowEntity = cType::InitializeType(
                0, 0, 0x251, type_gcDoCameraFollow,
                gcDoCameraFollowEntity::New, 0, 0, 0);
        }

        void *vt = ((void **)base)[1];
        const cType *myType = type_gcDoCameraFollowEntity;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)base + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = ((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            other = (const gcDoCameraFollowEntity *)base;
        }
    }

    // gcAction::operator= equivalent (bit fiddling at offset 8 + offset 0x14 copy)
    *(int *)((char *)this + 8) = *(int *)((char *)this + 8) & ~3;
    *(int *)((char *)this + 8) =
        *(int *)((char *)this + 8) | (*(const int *)((const char *)other + 8) & 3);
    *(int *)((char *)this + 0x14) = *(const int *)((const char *)other + 0x14);

    // gcDesiredCamera::operator= -> gcDesiredEntity::operator=
    gcDesiredEntity_op_eq((char *)this + 0x18, (const char *)other + 0x18);

    // gcDesiredValue at +0x44 with self-check + inline operator=
    if ((char *)other + 0x44 != (char *)this + 0x44) {
        int oldValue = *(int *)((char *)this + 0x44);
        int release = 1;
        int oldTagged = oldValue & 1;
        if (oldTagged != 0) {
            release = 0;
        }
        if (release != 0) {
            int releaseTagged = 0;
            if (oldTagged != 0) {
                releaseTagged = 1;
            }
            int stored;
            if (releaseTagged != 0) {
                stored = oldValue & -2;
            } else {
                stored = *(int *)oldValue;
            }
            *(int *)((char *)this + 0x44) = stored | 1;
            if (oldValue != 0) {
                void *classDesc = *(void **)(oldValue + 4);
                DtorDeleteRecord *entry =
                    (DtorDeleteRecord *)((char *)classDesc + 0x50);
                short offset = entry->offset;
                void (*fn)(void *, void *) = entry->fn;
                fn((char *)oldValue + offset, (void *)3);
            }
        }
        int sourceValue = *(const int *)((const char *)other + 0x44);
        int copy = 1;
        if ((sourceValue & 1) != 0) {
            copy = 0;
        }
        if (copy != 0) {
            void *classDesc = *(void **)(sourceValue + 4);
            CopyOpRec *entry = (CopyOpRec *)((char *)classDesc + 0x10);
            short offset = entry->offset;
            void *sourceObject = (char *)sourceValue + offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr((char *)this + 0x44);
            int currentValue = *(int *)((char *)this + 0x44);
            int currentTagged = 0;
            if ((currentValue & 1) != 0) {
                currentTagged = 1;
            }
            int currentObject;
            if (currentTagged != 0) {
                currentObject = currentValue & -2;
            } else {
                currentObject = *(int *)currentValue;
            }
            int (*fn)(void *, cMemPool *, int) = entry->fn;
            *(int *)((char *)this + 0x44) = fn(sourceObject, pool, currentObject);
        }
    }

    // Byte at 0x48
    *(unsigned char *)((char *)this + 0x48) =
        *(const unsigned char *)((const char *)other + 0x48);

    // gcDesiredObject::operator= -> gcDesiredEntity::operator=
    gcDesiredEntity_op_eq((char *)this + 0x4C, (const char *)other + 0x4C);

    // Int at 0x78
    *(int *)((char *)this + 0x78) = *(const int *)((const char *)other + 0x78);
}
