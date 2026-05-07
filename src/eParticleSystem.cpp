inline void *operator new(unsigned int, void *p) { return p; }

#include "mVec3.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

struct eParticleSystem_UpdateRec {
    short offset;
    short pad;
    void (*fn)(void *);
};

class cBase;
class cFile;
class cMemPool;
class cType;
class eDynamicGeom;
class eParticle;
class eParticleSystem;

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eDynamicGeom {
public:
    int base;

    eDynamicGeom(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eParticleSystem : public eDynamicGeom {
public:
    eParticleSystem(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    void SetAttractorPos(const mVec3 &);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct VTableSlot {
    short offset;
    short _pad;
    cType *(*getType)(void *);
};

extern char eParticleSystemvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046C44;

#pragma control sched=1

// ── eParticleSystem::eParticleSystem(cBase *) @ 0x0007b850 ──
eParticleSystem::eParticleSystem(cBase *parent) : eDynamicGeom(parent) {
    *(void **)((char *)this + 4) = eParticleSystemvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0x100) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = 0;
    *(int *)((char *)this + 0x10C) = 0;
    *(int *)((char *)this + 0x110) = 0;
}

// ── eParticleSystem::Write(cFile &) const @ 0x0007b748 ──
void eParticleSystem::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eParticleSystem::Read(cFile &, cMemPool *) @ 0x0007b794 ──
int eParticleSystem::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eParticleSystem::New(cMemPool *, cBase *) static @ 0x00214c88 ──
cBase *eParticleSystem::New(cMemPool *pool, cBase *parent) {
    eParticleSystem *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eParticleSystem *obj = (eParticleSystem *)rec->fn(base, 0x130, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eParticleSystem(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eParticleSystem::GetType(void) const @ 0x00214d04 ──
const cType *eParticleSystem::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_00046C44 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        __asm__ volatile("" : "+r"(parentType));
        __asm__ volatile("" ::: "memory");
        cBase *(*factory)(cMemPool *, cBase *) = eParticleSystem::New;
        __asm__ volatile("" : "+r"(factory));
        D_00046C44 = cType::InitializeType(0, 0, 0x30, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C44;
}

// ── eParticleSystem::AssignCopy(const cBase *) @ 0x002149a0 ──
void eParticleSystem::AssignCopy(const cBase *base) {
    const eParticleSystem *other = 0;

    if (base != 0) {
        if (D_00046C44 == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(
                            name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(
                        0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(
                    0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
            }
            const cType *parentType = D_000469C0;
            cBase *(*factory)(cMemPool *, cBase *) = &eParticleSystem::New;
            __asm__ volatile("" : "+r"(parentType), "+r"(factory));
            D_00046C44 = cType::InitializeType(
                0, 0, 0x30, parentType, factory, 0, 0, 0);
        }

        cType *target = D_00046C44;
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
                type = type->mParent;
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
            other = (const eParticleSystem *)base;
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
    float field5C = *(const float *)((const char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = field5C;

    *(int *)((char *)this + 0x60) =
        *(const int *)((const char *)other + 0x60);
    *(int *)((char *)this + 0x64) =
        *(const int *)((const char *)other + 0x64);
    *(int *)((char *)this + 0x68) =
        *(const int *)((const char *)other + 0x68);
    *(int *)((char *)this + 0x6C) =
        *(const int *)((const char *)other + 0x6C);
    *(int *)((char *)this + 0x70) =
        *(const int *)((const char *)other + 0x70);
    *(float *)((char *)this + 0x74) =
        *(const float *)((const char *)other + 0x74);
    *(float *)((char *)this + 0x78) =
        *(const float *)((const char *)other + 0x78);
    *(int *)((char *)this + 0x7C) =
        *(const int *)((const char *)other + 0x7C);
    *(int *)((char *)this + 0x80) =
        *(const int *)((const char *)other + 0x80);
    *(int *)((char *)this + 0x84) =
        *(const int *)((const char *)other + 0x84);
    *(int *)((char *)this + 0x88) =
        *(const int *)((const char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) =
        *(const unsigned char *)((const char *)other + 0x8D);

    *(v4sf_t *)((char *)this + 0xC0) =
        *(const v4sf_t *)((const char *)other + 0xC0);
    *(v4sf_t *)((char *)this + 0x90) =
        *(const v4sf_t *)((const char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) =
        *(const v4sf_t *)((const char *)other + 0xA0);
    *(v4sf_t *)((char *)this + 0xB0) =
        *(const v4sf_t *)((const char *)other + 0xB0);

    *(short *)((char *)this + 0xD0) =
        *(const short *)((const char *)other + 0xD0);
    *(unsigned char *)((char *)this + 0xD2) =
        *(const unsigned char *)((const char *)other + 0xD2);
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0xD4) =
        *(const float *)((const char *)other + 0xD4);
    __asm__ volatile("" ::: "memory");
    int *dstD8 = (int *)((char *)this + 0xD8);
    const int *srcD8 = (const int *)((const char *)other + 0xD8);
    *dstD8 = *srcD8;
    *(int *)((char *)this + 0xDC) =
        *(const int *)((const char *)other + 0xDC);
    *(int *)((char *)this + 0xE0) =
        *(const int *)((const char *)other + 0xE0);
    *(int *)((char *)this + 0xE4) =
        *(const int *)((const char *)other + 0xE4);
    *(int *)((char *)this + 0xE8) =
        *(const int *)((const char *)other + 0xE8);

    ((cArrayBase<eParticle> *)((char *)this + 0xF0))->operator=(
        *(const cArrayBase<eParticle> *)((const char *)other + 0xF0));

    *(int *)((char *)this + 0x100) =
        *(const int *)((const char *)other + 0x100);
    int *dst104 = (int *)((char *)this + 0x104);
    const int *src104 = (const int *)((const char *)other + 0x104);
    *dst104 = *src104;
    int *dst108 = (int *)((char *)this + 0x108);
    const int *src108 = (const int *)((const char *)other + 0x108);
    *dst108 = *src108;
    int *dst10C = (int *)((char *)this + 0x10C);
    const int *src10C = (const int *)((const char *)other + 0x10C);
    *dst10C = *src10C;
    *(int *)((char *)this + 0x110) =
        *(const int *)((const char *)other + 0x110);

    *(v4sf_t *)((char *)this + 0x120) =
        *(const v4sf_t *)((const char *)other + 0x120);
}

void eParticleSystem::SetAttractorPos(const mVec3 &pos) {
    unsigned char *attractor = *(unsigned char **)((unsigned char *)this + 0x60);
    unsigned char secondary = *(unsigned char *)(attractor + 0x4D);
    unsigned char *ocs;
    unsigned char *self = (unsigned char *)this;
    const unsigned char *src = (const unsigned char *)&pos;

    if (secondary == 0 && *(unsigned char *)(attractor + 0x4C) == 0) {
        if (*(unsigned char *)(self + 0x8C) & 4) {
            unsigned char *vtab = (unsigned char *)((void **)self)[1];
            eParticleSystem_UpdateRec *rec =
                (eParticleSystem_UpdateRec *)(vtab + 0xB8);
            short off = rec->offset;
            void (*fn)(void *) = rec->fn;
            fn(self + off);
        }

        ocs = self + 0x10;
        v4sf_t scratch;
        v4sf_t out;
        __asm__ volatile(
            "lv.q %1, 0x0(%2)\n"
            "lv.q C000, 0x0(%3)\n"
            "lv.q C010, 0x10(%3)\n"
            "lv.q C020, 0x20(%3)\n"
            "lv.q C030, 0x30(%3)\n"
            "vsub.t %1, %1, C030\n"
            "vtfm3.t %0, M000, %1"
            : "=v"(out), "=v"(scratch)
            : "r"(src), "r"(ocs)
            : "memory");
        *(v4sf_t *)(self + 0x120) = out;

        if (attractor != 0 && *(unsigned char *)(attractor + 0x4C) != 0) {
            if (*(unsigned char *)(self + 0x8C) & 4) {
                unsigned char *vtab = (unsigned char *)((void **)self)[1];
                eParticleSystem_UpdateRec *rec =
                    (eParticleSystem_UpdateRec *)(vtab + 0xB8);
                short off = rec->offset;
                void (*fn)(void *) = rec->fn;
                fn(self + off);
            }

            v4sf_t scratch2;
            v4sf_t out2;
            __asm__ volatile(
                "lv.q %1, 0x0(%2)\n"
                "lv.q C000, 0x0(%3)\n"
                "lv.q C010, 0x10(%3)\n"
                "lv.q C020, 0x20(%3)\n"
                "lv.q C030, 0x30(%3)\n"
                "vsub.t %1, %1, C030\n"
                "vtfm3.t %0, M000, %1"
                : "=v"(out2), "=v"(scratch2)
                : "r"(src), "r"(ocs)
                : "memory");
            *(v4sf_t *)(attractor + 0x170) = out2;
        }
    }
}
