#include "cObject.h"

class cBase;
class cFilename;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

extern "C" void eHeightmapTemplateData___dtor_eHeightmapTemplateData_void(void *, int);

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
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
    void PlatformFree(void);
    float GetRadius(void) const;
    void GetExternalDependency(int, cFilename *) const;
    cStr GetRelativeFilename(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char eHeightmapTemplatevirtualtable[];

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

// ── GetExternalDependency @ 0x001f58e0 ──
void eHeightmapTemplate::GetExternalDependency(int, cFilename *out) const {
    cStr filename;
    filename = GetRelativeFilename();
    ((cStr *)out)->Set((const char *)&filename);
}

// ── GetRadius @ 0x001f5964 ──
float eHeightmapTemplate::GetRadius(void) const {
    float extent = (float)(*(int *)((char *)this + 0x68) - 1) * *(float *)((char *)this + 0x70);
    float z = *(float *)((char *)this + 0x74);
    float result;
    __asm__ volatile(
        "vzero.t C120\n"
        "mfc1 $a0, %1\n"
        "mfc1 $a1, %1\n"
        "mfc1 $a2, %2\n"
        "mtv $a0, S130\n"
        "mtv $a1, S131\n"
        "mtv $a2, S132\n"
        "vsub.t C120, C130, C120\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"
        : "=f"(result)
        : "f"(extent), "f"(z)
        : "a0", "a1", "a2"
    );
    return result * 0.5f;
}
