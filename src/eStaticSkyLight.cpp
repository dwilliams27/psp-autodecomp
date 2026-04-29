#include "eStaticSkyLight.h"
#include "mVec3.h"
#include "mRay.h"
#include "eBipedController.h"

class cFile;
class cMemPool;
class cBase;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

template <class T> T *dcast(const cBase *);

void *cMemPool_GetPoolFromPtr(void *);

extern "C" {
    void eStaticLight___dtor_eStaticLight_void(void *, int);
    void eStaticSkyLight_eStaticSkyLight(eStaticSkyLight *, cBase *);
}

extern char eStaticSkyLightvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046B30;
extern cType *D_00046B38;

#pragma control sched=1

struct cHandle {
    int mIndex;
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

// ── eStaticSkyLight::eStaticSkyLight(cBase *) @ 0x0005F2CC ──
eStaticSkyLight::eStaticSkyLight(cBase *parent) : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticSkyLightvirtualtable;
    *(float *)((char *)this + 0x90) = 180.0f;
    __asm__ volatile("" ::: "memory");
}

// ── eStaticSkyLight::Write(cFile &) const @ 0x0005F1A4 ──
void eStaticSkyLight::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eStaticLight::Write(file);
    wb.Write(*(float *)((char *)this + 0x90));
    wb.End();
}

// ── eStaticSkyLight::~eStaticSkyLight(void) @ 0x0005F30C ──
extern "C" {

void eStaticSkyLight___dtor_eStaticSkyLight_void(eStaticSkyLight *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eStaticSkyLightvirtualtable;
        eStaticLight___dtor_eStaticLight_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}

}

// ── eStaticSkyLight::New(cMemPool *, cBase *) static @ 0x002060C0 ──
cBase *eStaticSkyLight::New(cMemPool *pool, cBase *parent) {
    eStaticSkyLight *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eStaticSkyLight *obj = (eStaticSkyLight *)entry->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eStaticSkyLight_eStaticSkyLight(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eStaticSkyLight::GetType(void) const {
    if (D_00046B38 == 0) {
        if (D_00046B30 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *kindName = (const char *)0x36CEE0;
            const char *kindDesc = (const char *)0x36CEEC;
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_00046B30 = cType::InitializeType(0, 0, 0x4A, parentType,
                                               0, kindName, kindDesc, 0);
        }
        const cType *parentType = D_00046B30;
        cBase *(*factory)(cMemPool *, cBase *) = &eStaticSkyLight::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046B38 = cType::InitializeType(0, 0, 0x4E, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046B38;
}

// ── eStaticSkyLight::AssignCopy(const cBase *) @ 0x00206040 ──
void eStaticSkyLight::AssignCopy(const cBase *base) {
    eStaticSkyLight *other = dcast<eStaticSkyLight>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    *(cHandle *)((char *)this + 0x48) = *(const cHandle *)((const char *)other + 0x48);
    __asm__ volatile(
        "lv.q C120, 0x80(%1)\n"
        "sv.q C120, 0x80(%0)\n"
        "lv.q C120, 0x50(%1)\n"
        "sv.q C120, 0x50(%0)\n"
        "lv.q C120, 0x60(%1)\n"
        "sv.q C120, 0x60(%0)\n"
        "lv.q C120, 0x70(%1)\n"
        "sv.q C120, 0x70(%0)\n"
        :: "r"(this), "r"(other) : "memory"
    );
    *(float *)((char *)this + 0x90) = *(const float *)((const char *)other + 0x90);
}

#pragma control sched=2

int eStaticSkyLight::GetNumStratifiedSamples(void) const {
    return 8;
}

void eStaticSkyLight::GetDirectLight(mVec3 *out, const mVec3 &, const mVec3 &normal, const mRay &ray, const mVec3 &) const {
    volatile float color[8];

    float zero, dot;
    __asm__ volatile(
        "lv.q C120, 0x10(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a2, S100\n"
        "mtc1 $a2, %1\n"
        "mtc1 $zero, %0\n"
        : "=f"(zero), "=f"(dot)
        : "r"(&ray), "r"(&normal)
        : "$a2"
    );

    if (dot > zero) {
        float scale = 1.0f / 255.0f;
        color[0] = (float)colorR * scale;
        color[1] = (float)colorG * scale;
        color[2] = (float)colorB * scale;
        float factor = dot * intensity;

        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mtv $a0, S100\n"
            "lv.q C120, 0($sp)\n"
            "vscl.t C120, C120, S100\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(factor), "r"(out)
            : "$a0", "memory"
        );
        return;
    }

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a2, %0\n"
        "mfc1 $a3, %0\n"
        "mtv $a0, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(zero), "r"(out)
        : "$a0", "$a2", "$a3", "memory"
    );
}

void eBipedController::GetVelocity(int, mVec3 *vel, mVec3 *accel) const {
    int *entry = (int *)((char *)bodyEntries + bodyIndex * 8);

    float fzero = 0.0f;
    unsigned char found = 0;
    if (entry[0] != 0) {
        if (entry[1] != 0) {
            found = 1;
        }
        found = (unsigned char)(found & 0xff);
    }

    if (found) goto found_path;

    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a1, %0\n"
        "mfc1 $t0, %0\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $t0, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(fzero), "r"(vel)
        : "$a0", "$a1", "$t0", "memory"
    );
    __asm__ volatile(
        "mfc1 $a0, %0\n"
        "mfc1 $a1, %0\n"
        "mfc1 $a2, %0\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0(%1)\n"
        :: "f"(fzero), "r"(accel)
        : "$a0", "$a1", "$a2", "memory"
    );
    return;

found_path:
    {
        void *body = (void *)entry[0];
        __asm__ volatile(
            "lv.q C120, 0x40(%0)\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(body), "r"(vel)
            : "memory"
        );
        __asm__ volatile(
            "mfc1 $a0, %0\n"
            "mfc1 $a1, %0\n"
            "mfc1 $a2, %0\n"
            "mtv $a0, S120\n"
            "mtv $a1, S121\n"
            "mtv $a2, S122\n"
            "sv.q C120, 0(%1)\n"
            :: "f"(fzero), "r"(accel)
            : "$a0", "$a1", "$a2", "memory"
        );
    }
}
