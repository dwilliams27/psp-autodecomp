#include "mOCS.h"

class cFile;
class cMemPool;
class cBase;
class eCollisionInfo;
class mRay;
class mCollideHit;
class mSphere;
class mCollideInfo;
class eContactCollector;
class eSurface;

typedef int v4sf_t __attribute__((mode(V4SF)));
typedef int v16sf_t __attribute__((mode(V16SF)));

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

class eGeom {
public:
    static eGeom *s_pFirstUpdate;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void CleanNeedsUpdateList(void);
    void ApplyLocalToWorld(const mOCS &);
    eGeom(cBase *base);

    bool CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    bool CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    int GetSweptContacts(const eCollisionInfo &, int, const mSphere *,
                         const mCollideInfo *, eContactCollector *) const;
    int GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
                         eContactCollector *) const;
    eSurface *GetSurface(int) const;
};

extern char eGeomvirtualtable[];

// ── Write ──
void eGeom::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// ── Read ──
int eGeom::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] == 1)
        goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── CleanNeedsUpdateList ──
void eGeom::CleanNeedsUpdateList(void) {
    eGeom *g = s_pFirstUpdate;
    if (g != 0) {
        do {
            int *entry = (int *)((char *)*(int **)((char *)g + 4) + 0xB8);
            short adj = *(short *)entry;
            void (*fn)(void *) = (void (*)(void *))entry[1];
            fn((char *)g + adj);
            g = s_pFirstUpdate;
        } while (g != 0);
    }
}

// ── ApplyLocalToWorld ──
void eGeom::ApplyLocalToWorld(const mOCS &ocs) {
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)&ocs + 0x30);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)&ocs + 0x00);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)&ocs + 0x10);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)&ocs + 0x20);
    int *entry = (int *)((char *)*(int **)((char *)this + 4) + 0x70);
    short adj = *(short *)entry;
    void (*fn)(void *, int, int, int) = (void (*)(void *, int, int, int))entry[1];
    fn((char *)this + adj, 4, 0, adj);
}

// ── eGeom constructor ──
eGeom::eGeom(cBase *base) {
    *(cBase **)this = base;
    *(void **)((char *)this + 4) = eGeomvirtualtable;
    *(int *)((char *)this + 0x60) = 0;
    *(int *)((char *)this + 0x64) = 0;
    *(int *)((char *)this + 0x68) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(float *)((char *)this + 0x74) = -1.0f;
    *(float *)((char *)this + 0x78) = 1.0f;
    *(int *)((char *)this + 0x7C) = 0;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
    *(int *)((char *)this + 0x88) = 0;
    *(unsigned char *)((char *)this + 0x8C) = 0xE0;
    *(unsigned char *)((char *)this + 0x8D) = 0;
    __asm__ volatile(
        "vmidt.q M000\n"
        "vmov.q C120, C000\n"
        "vmov.q C130, C010\n"
        "vmov.q C200, C020\n"
        "vmov.q C210, C030\n"
        "sv.q C120, 0x10(%0)\n"
        "sv.q C130, 0x20(%0)\n"
        "sv.q C200, 0x30(%0)\n"
        "sv.q C210, 0x40(%0)\n"
        : : "r"(this) : "memory"
    );
    *(float *)((char *)this + 0x5C) = -1.0f;
}

// ── CastRay ──
bool eGeom::CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const {
    return false;
}

// ── CastSphere ──
bool eGeom::CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const {
    return false;
}

// ── GetSweptContacts ──
int eGeom::GetSweptContacts(const eCollisionInfo &, int, const mSphere *,
                            const mCollideInfo *, eContactCollector *) const {
    return 0;
}

// ── GetEmbedContacts ──
int eGeom::GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
                            eContactCollector *) const {
    return 0;
}

// ── GetSurface ──
eSurface *eGeom::GetSurface(int) const {
    return 0;
}
