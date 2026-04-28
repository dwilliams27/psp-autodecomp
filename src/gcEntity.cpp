#include "gcEntity.h"

extern const mVec3 mVec3_Zero;

inline void *operator new(unsigned int, void *p) { return p; }

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
