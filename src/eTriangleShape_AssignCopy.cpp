// ODR-WARNING: split-TU local redeclaration for one eTriangleShape method.

class cBase;
class cMemPool;
class cType;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eTriangleShape {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct cTypeNode {
    char pad[0x1C];
    cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BDC;

#pragma control sched=1

void eTriangleShape::AssignCopy(const cBase *base) {
    const eTriangleShape *other = 0;

    if (base != 0) {
        if (D_00046BDC == 0) {
            if (D_00040FE4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FE4 = cType::InitializeType(0, 0, 0x227,
                                                   D_000385DC, 0, 0, 0, 0);
            }
            const cType *parentType = D_00040FE4;
            cBase *(*factory)(cMemPool *, cBase *) =
                (cBase *(*)(cMemPool *, cBase *))0x20AC98;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046BDC = cType::InitializeType(0, 0, 0x2D5, parentType,
                                               factory, 0, 0, 0);
        }

        cType *target = D_00046BDC;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        VTableSlot *slot = (VTableSlot *)((char *)classDesc + 8);
        short offset = slot->offset;
        cType *type = slot->getType((void *)((const char *)base + offset));
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
                type = ((cTypeNode *)type)->parent;
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
            other = (const eTriangleShape *)base;
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
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    __asm__ volatile("" ::: "memory");

    unsigned int i = 0;
    char *vecDst = (char *)this + 0x80;
    const char *vecSrc = (const char *)other + 0x80;
    do {
        *(v4sf_t *)vecDst = *(const v4sf_t *)vecSrc;
        i++;
        vecDst += 0x10;
        vecSrc += 0x10;
    } while (i < 3U);

    *(int *)((char *)this + 0xB0) =
        *(const int *)((const char *)other + 0xB0);
}
