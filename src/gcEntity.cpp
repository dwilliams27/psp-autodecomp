#include "gcEntity.h"

extern const mVec3 mVec3_Zero;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009A408;

struct gcEntityAllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcEntity::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    gcEntityAllocRec *rec = (gcEntityAllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntity *result = 0;
    gcEntity *obj = (gcEntity *)rec->fn(base, 0x260, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcEntity(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcEntity::GetType(void) const {
    if (D_0009A408 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009A408 = cType::InitializeType(0, 0, 0x8C, D_000385E4,
                                           &gcEntity::New,
                                           (const char *)0x36D968,
                                           (const char *)0x36D974, 0);
    }
    return D_0009A408;
}

void gcEntity::AssignCopy(const cBase *base) {
    const gcEntity *other = 0;

    if (base != 0) {
        if (D_0009A408 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_0009A408 = cType::InitializeType(0, 0, 0x8C, D_000385E4,
                                               &gcEntity::New,
                                               (const char *)0x36D968,
                                               (const char *)0x36D974, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A408;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcEntity *)base;
        }
    }
    operator=(*other);
}

gcPlayer *gcEntity::GetPlayer(void) const {
    const gcEntity *entity = this;
    if (entity != 0) {
        int index = *(signed char *)((char *)entity + 0x67);
    check_index:
        if (index < 0) {
            goto next_entity;
        }
        gcPlayer *result = 0;
        if (index >= 0 && index < 8) {
            result = (gcPlayer *)(*(char **)0x37D87C + index * 68);
        }
        return result;
    next_entity:
        entity = *(const gcEntity **)((char *)entity + 0x218);
        if (entity != 0) {
            index = *(signed char *)((char *)entity + 0x67);
        } else {
            return 0;
        }
        goto check_index;
    }
    return 0;
}

void gcEntity::OnFullscreenCinematicStarting(void) {
    if ((unsigned char)((*(unsigned int *)0x37D858 & 2) != 0)) {
        void **globals = (void **)0x380000;
        void *entity = globals[-2533];
        if (entity != 0) {
            do {
                int flags = *(int *)((char *)entity + 0x5C);
                if ((flags & 0x80) != 0) {
                    *(int *)((char *)entity + 0x5C) = flags & 0xFFFDFFFF;
                }
                entity = *(void **)((char *)entity + 0x228);
            } while (entity != globals[-2533]);
        }
    }
}

void gcEntity::SetVelocity(const mVec3 &vel) {
    mVec3 *body = *(mVec3 **)((char *)this + 0x1F8);
    if (body != 0) {
        *body = vel;
    }
}

void *gcEntity::GetSubGeomController(int index) {
    if (index < 0) {
        return 0;
    }
    void **arr = *(void ***)((char *)this + 0x1F0);
    int count = 0;
    if (arr != 0) {
        count = *((int *)arr - 1);
    }
    if (index < count) {
        return arr[index];
    }
    return 0;
}

float gcEntity::GetSoundFrequencyOffset(int index) const {
    int flags = *(int *)((char *)this + 0x5C);
    int inactive = 1;
    if (flags & 1) inactive = 0;
    if (inactive) {
        return 0.0f;
    }
    if (index < 0) goto fail;
    {
        int count = 0;
        void **arr = *(void ***)((char *)this + 0x1D8);
        if (arr != 0) count = *((int *)arr - 1);
        if (index < count) goto success;
    fail:
        return 0.0f;
    success:
        return *(float *)((char *)arr[index] + 0x34);
    }
}

void gcEntity::GetVelocity(mVec3 *out) const {
    mVec3 *body = *(mVec3 **)((char *)this + 0x1F8);
    if (body == 0) {
        mVec3 zero = mVec3_Zero;
        body = &zero;
    }
    *out = *body;
}
