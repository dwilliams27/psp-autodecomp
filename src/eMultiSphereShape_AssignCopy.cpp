// ODR-WARNING: local redeclaration for a split-TU method addition.
// Keep this class shape limited to methods referenced in this translation unit.

class cBase;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class eMultiSphereShape {
public:
    void AssignCopy(const cBase *);
    static eMultiSphereShape *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BBC;

#pragma control sched=1

void eMultiSphereShape::AssignCopy(const cBase *base) {
    const eMultiSphereShape *other = 0;

    if (base != 0) {
        if (D_00046BBC == 0) {
            if (D_00040FE4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FE4 = cType::InitializeType(
                    0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
            }
            const cType *parentType = D_00040FE4;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x20936C;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046BBC = cType::InitializeType(
                0, 0, 0x1DB, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046BBC;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
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
            other = (const eMultiSphereShape *)base;
        }
    }

    *(v4sf_t *)((char *)this + 0x40) =
        *(const v4sf_t *)((const char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) =
        *(const v4sf_t *)((const char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) =
        *(const v4sf_t *)((const char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) =
        *(const v4sf_t *)((const char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) =
        *(const unsigned char *)((const char *)other + 0x50);
    __asm__ volatile("" ::: "memory");

    int *dst = (int *)((char *)this + 0x54);
    const int *src = (const int *)((const char *)other + 0x54);
    int word0 = src[0];
    int word1 = src[1];
    int word2 = src[2];
    dst[0] = word0;
    dst[1] = word1;
    dst[2] = word2;
    int word3 = src[3];
    int word4 = src[4];
    int word5 = src[5];
    dst[3] = word3;
    dst[4] = word4;
    dst[5] = word5;

    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(float *)((char *)this + 0x80) =
        *(const float *)((const char *)other + 0x80);
    *(float *)((char *)this + 0x84) =
        *(const float *)((const char *)other + 0x84);
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);

    unsigned int i = 0;
    char *vecDst = (char *)this + 0x90;
    const char *vecSrc = (const char *)other + 0x90;
    do {
        float last = ((const float *)vecSrc)[3];
        *(v4sf_t *)vecDst = *(const v4sf_t *)vecSrc;
        ((float *)vecDst)[3] = last;
        __asm__ volatile("" ::: "memory");
        i++;
        vecDst += 0x10;
        vecSrc += 0x10;
    } while (i < 4U);
}

#pragma control sched=2
