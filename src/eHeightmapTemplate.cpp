#include "cObject.h"

class cBase;
class cFilename;
class cMemPool;
class eHeightmapTile;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eHeightmap {
public:
    static cBase *New(cMemPool *, cBase *);
};

inline void *operator new(unsigned int, void *p) { return p; }

extern "C" void eHeightmapTemplateData___dtor_eHeightmapTemplateData_void(void *, int);

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
};

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eHeightmapTemplate : public cObject {
public:
    eHeightmapTemplate(cBase *);
    void AssignCopy(const cBase *);
    void PlatformFree(void);
    float GetRadius(void) const;
    void GetExternalDependency(int, cFilename *) const;
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    cStr GetRelativeFilename(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char eHeightmapTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF4;
extern cType *D_000469A4;
extern cType *D_000469A8;
extern cType *D_000469AC;
extern cType *D_00046A0C;
extern cType *D_00046A10;

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

struct eHeightmapTemplate_block_24 { int _[6]; };
struct eHeightmapTemplate_block_4 { int _[1]; };

// ── PlatformFree @ 0x000520b8 ──
void eHeightmapTemplate::PlatformFree(void) {
    void *data = *(void **)((char *)this + 0x80);
    if (data != 0) {
        eHeightmapTemplateData___dtor_eHeightmapTemplateData_void(data, 3);
        *(void **)((char *)this + 0x80) = 0;
    }
}

// ── Constructor @ 0x000520f0 ──
eHeightmapTemplate::eHeightmapTemplate(cBase *base) : cObject(base) {
    *(void **)((char *)this + 4) = eHeightmapTemplatevirtualtable;
    *(short *)((char *)this + 0x58) = 0;
    *(short *)((char *)this + 0x5A) = 0;
    *(char *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x5C) = 0;
    *(int *)((char *)this + 0x60) = 0;
    *(int *)((char *)this + 0x64) = 0;
    *(int *)((char *)this + 0x68) = 0x41;
    *(int *)((char *)this + 0x6C) = 0x10;
    *(float *)((char *)this + 0x70) = 4.0f;
    *(int *)((char *)this + 0x7C) = 0;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
    *(int *)((char *)this + 0x88) = 0;
}

// ── New @ 0x001f56cc ──
cBase *eHeightmapTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eHeightmapTemplate *result = 0;
    eHeightmapTemplate *obj = (eHeightmapTemplate *)entry->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eHeightmapTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1
const cType *eHeightmapTemplate::GetType(void) const {
    if (D_00046A0C == 0) {
        if (D_000469AC == 0) {
            if (D_000469A8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CE2C;
                const char *kindDesc = (const char *)0x36CE3C;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, parentType, 0, kindName, kindDesc, 5);
            }
            D_000469AC = cType::InitializeType(0, 0, 0x21, D_000469A8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469AC;
        cBase *(*factory)(cMemPool *, cBase *) = &eHeightmapTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046A0C = cType::InitializeType(0, 0, 0x54, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046A0C;
}
#pragma control sched=2

// ── GetInstanceType @ 0x00052380 ──
#pragma control sched=1
const cType *eHeightmapTemplate::GetInstanceType(void) const {
    if (D_00046A10 == 0) {
        if (D_000469A4 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469A4 = cType::InitializeType(0, 0, 0x1A, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469A4;
        cBase *(*factory)(cMemPool *, cBase *) = &eHeightmap::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046A10 = cType::InitializeType(0, 0, 0x55, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046A10;
}
#pragma control sched=2

// ── AssignCopy @ 0x001f55e8 ──
#pragma control sched=1
void eHeightmapTemplate::AssignCopy(const cBase *src) {
    eHeightmapTemplate *other = dcast<eHeightmapTemplate>(src);
    cObject::operator=(*other);
    __asm__ volatile("" ::: "memory");
    *(eHeightmapTemplate_block_24 *)((char *)this + 0x44) =
        *(eHeightmapTemplate_block_24 *)((char *)other + 0x44);
    *(int *)((char *)this + 0x5C) = *(int *)((char *)other + 0x5C);
    *(int *)((char *)this + 0x60) = *(int *)((char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(int *)((char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(int *)((char *)other + 0x68);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(float *)((char *)this + 0x70) = *(float *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<eHeightmapTile> *)((char *)this + 0x7C))->operator=(
        *(const cArrayBase<eHeightmapTile> *)((char *)other + 0x7C));
    *(int *)((char *)this + 0x80) = *(int *)((char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(int *)((char *)other + 0x84);
    __asm__ volatile("" ::: "memory");
    eHeightmapTemplate_block_4 *dst88 = (eHeightmapTemplate_block_4 *)((char *)this + 0x88);
    eHeightmapTemplate_block_4 *src88 = (eHeightmapTemplate_block_4 *)((char *)other + 0x88);
    *dst88 = *src88;
}
#pragma control sched=2

// ── GetExternalDependency @ 0x001f58e0 ──
#pragma control sched=1
void eHeightmapTemplate::GetExternalDependency(int, cFilename *out) const {
    register cFilename *target __asm__("$16");
    register const eHeightmapTemplate *self __asm__("$5");
    __asm__ volatile(
        "or %0, %2, $0\n"
        "or %1, %3, $0"
        : "=r"(target), "=r"(self)
        : "r"(out), "r"(this));
    cStr filename = self->GetRelativeFilename();
    ((cStr *)target)->Set((const char *)&filename);
}
#pragma control sched=2

// ── GetRadius @ 0x001f5964 ──
float eHeightmapTemplate::GetRadius(void) const {
    float extent = (float)(*(int *)((char *)this + 0x68) - 1) * *(float *)((char *)this + 0x70);
    float result;
    __asm__ volatile(
        "vzero.t C120\n"
        "lwc1 $f14, 0x74(%2)\n"
        "mfc1 $a0, %1\n"
        "mfc1 $a1, %1\n"
        "mfc1 $a2, $f14\n"
        "mtv $a0, S130\n"
        "mtv $a1, S131\n"
        "mtv $a2, S132\n"
        "vsub.t C120, C130, C120\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"
        : "=f"(result)
        : "f"(extent), "r"(this)
        : "a0", "a1", "a2"
    );
    return result * 0.5f;
}
