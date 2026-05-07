#include "mOCS.h"

class cFile;
class cMemPool;
class cBase;
class cType;
class eCollisionInfo;
class mRay;
class mCollideHit;
class mSphere;
class mCollideInfo;
class eContactCollector;
class eSurface;
class eGeomTemplate;
class eWorld;
class eGeom;
template <class T> class cHandleT;

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool_shim {
public:
    static void *GetPoolFromPtr(const void *);
};

class eWorld {
public:
    enum eWorldGeomBin {
        eWorldGeomBin_Decal = 2
    };

    void RemoveBinGeom(eGeom *, eWorldGeomBin);
    void UpdateGeomLocation(eGeom *);
};

class eRoom {
public:
    void RemoveGeom(eGeom *);
};

class eGeom {
public:
    static eGeom *s_pFirstUpdate;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void CleanNeedsUpdateList(void);
    void ApplyLocalToWorld(const mOCS &);
    eGeom(cBase *base);

    void OnAddedToWorld(void);
    void OnRemovedFromWorld(void);
    const cType *GetType(void) const;
    void SetTemplate(cHandleT<eGeomTemplate>, bool);
    void SetGeomFlagsOnOff(unsigned int, unsigned int);
    void UpdateLocalToWorld(void);

    bool CastRay(const eCollisionInfo &, const mRay &, mCollideHit *) const;
    bool CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
    int GetSweptContacts(const eCollisionInfo &, int, const mSphere *,
                         const mCollideInfo *, eContactCollector *) const;
    int GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
                         eContactCollector *) const;
    eSurface *GetSurface(int) const;
};

class eSweptVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &);
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

extern char eGeomvirtualtable[];

extern const char eGeom_cBase_name[];   // 0x36CD74 = "cBase"
extern const char eGeom_cBase_desc[];   // 0x36CD7C = "Base"

static cType *type_base;
static cType *type_eGeom;

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

// ── OnAddedToWorld ──
void eGeom::OnAddedToWorld(void) {
}

// ── OnRemovedFromWorld ──
void eGeom::OnRemovedFromWorld(void) {
}

// ── GetType ──
const cType *eGeom::GetType(void) const {
    if (!type_eGeom) {
        if (!type_base) {
            type_base = cType::InitializeType(eGeom_cBase_name, eGeom_cBase_desc, 1, 0, 0, 0, 0, 0);
        }
        type_eGeom = cType::InitializeType(0, 0, 0x16, type_base, 0, 0, 0, 0);
    }
    return type_eGeom;
}

// ── SetTemplate ──
struct VirtCallEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *, int);
};

void eGeom::SetTemplate(cHandleT<eGeomTemplate> templ, bool reset) {
    void *templPtr;
    if (templ.mIndex == 0) {
        templPtr = 0;
    } else {
        templPtr = 0;
        void *entry = ((void **)0x38890)[(unsigned int)templ.mIndex & 0xFFFF];
        if (entry != 0 && *(int *)((char *)entry + 0x30) == templ.mIndex) {
            templPtr = entry;
        }
    }
    *(void **)((char *)this + 0x60) = templPtr;
    if (reset) {
        VirtCallEntry *e = (VirtCallEntry *)((char *)*(void **)((char *)this + 4) + 0x38);
        short adj = e->offset;
        void *base = (char *)this + adj;
        void *pool = cMemPool_shim::GetPoolFromPtr(this);
        e->fn(base, pool, 1);
    }
}

// ── UpdateLocalToWorld ──
void eGeom::UpdateLocalToWorld(void) {
    *((unsigned char *)this + 0x8C) = *((unsigned char *)this + 0x8C) & ~4;
    if ((*(eGeom **)((char *)this + 0x80) != 0 || *(eGeom **)((char *)this + 0x84) != 0)
        && this != 0 && *(eGeom **)((char *)this + 0x80) != 0) {
        if (*(eGeom **)((char *)this + 0x84) != 0) {
            if (s_pFirstUpdate == this) {
                s_pFirstUpdate = *(eGeom **)((char *)this + 0x84);
            }
            *(eGeom **)((char *)*(eGeom **)((char *)this + 0x80) + 0x84) = *(eGeom **)((char *)this + 0x84);
            *(eGeom **)((char *)*(eGeom **)((char *)this + 0x84) + 0x80) = *(eGeom **)((char *)this + 0x80);
            *(eGeom **)((char *)this + 0x80) = 0;
            *(eGeom **)((char *)this + 0x84) = 0;
            if (s_pFirstUpdate == this) {
                s_pFirstUpdate = 0;
            }
        }
    }
    if (*((unsigned char *)this + 0x8C) & 1) {
        eWorld *world = *(eWorld **)((char *)this + 0x64);
        if (world != 0) {
            world->UpdateGeomLocation(this);
        }
    }
}

// ── eSweptVolumeCollisionHandler::SetLocalToWorld ──
void eSweptVolumeCollisionHandler::SetLocalToWorld(const mOCS &ocs) {
    SceULong128 p = *(SceULong128 *)((char *)this + 0x40);
    __asm__ volatile(
        "lv.q C000, 0x0(%1)\n"
        "lv.q C010, 0x10(%1)\n"
        "lv.q C020, 0x20(%1)\n"
        "lv.q C030, 0x30(%1)\n"
        "vsub.t %0, %0, C030\n"
        "vtfm3.t C130, M000, %0\n"
        "sv.q C130, 0x80(%2)\n"
        : "+X"(p)
        : "r"(&ocs), "r"(this)
        : "memory"
    );

    p = *(SceULong128 *)((char *)this + 0x50);
    __asm__ volatile(
        "lv.q C000, 0x0(%1)\n"
        "lv.q C010, 0x10(%1)\n"
        "lv.q C020, 0x20(%1)\n"
        "lv.q C030, 0x30(%1)\n"
        "vsub.t %0, %0, C030\n"
        "vtfm3.t C130, M000, %0\n"
        "sv.q C130, 0x90(%2)\n"
        : "+X"(p)
        : "r"(&ocs), "r"(this)
        : "memory"
    );

    __asm__ volatile(
        "lv.q C120, 0x60(%1)\n"
        "lv.q C000, 0x0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, M000, C120\n"
        "sv.q C130, 0xa0(%1)\n"
        :
        : "r"(&ocs), "r"(this)
        : "memory"
    );

    *(float *)((char *)this + 0xb0) = *(float *)((char *)this + 0x70);
    *(SceULong128 *)((char *)this + 0x120) = ocs.position;
    *(SceULong128 *)((char *)this + 0xf0) = ocs.row0;
    *(SceULong128 *)((char *)this + 0x100) = ocs.row1;
    *(SceULong128 *)((char *)this + 0x110) = ocs.row2;
}

// ── SetGeomFlagsOnOff ──
void eGeom::SetGeomFlagsOnOff(unsigned int on, unsigned int off) {
    unsigned int flags = *(unsigned char *)((char *)this + 0x8C);
    unsigned int newFlags = (flags | on) & ~off;
    if (newFlags != flags) {
        if ((newFlags & 4) && *(void **)((char *)this + 0x64) != 0) {
            if (*(void **)((char *)this + 0x80) == 0
                || *(void **)((char *)this + 0x84) == 0) {
                eGeom *head = s_pFirstUpdate;
                if (head != 0) {
                    *(void **)((char *)this + 0x80) = *(void **)((char *)head + 0x80);
                    *(eGeom **)((char *)this + 0x84) = s_pFirstUpdate;
                    *(eGeom **)((char *)*(void **)((char *)this + 0x80) + 0x84) = this;
                    *(eGeom **)((char *)*(void **)((char *)this + 0x84) + 0x80) = this;
                } else {
                    s_pFirstUpdate = this;
                    *(eGeom **)((char *)this + 0x84) = this;
                    *(eGeom **)((char *)this + 0x80) = this;
                }
                flags = *(unsigned char *)((char *)this + 0x8C);
            }
        }

        int was = 0;
        if (flags & 1) was = 1;
        int will = 0;
        if (newFlags & 1) will = 1;
        *(unsigned char *)((char *)this + 0x8C) = newFlags;

        if (will != was) {
            if (will != 0) {
                float area = *(float *)((char *)this + 0x74) *
                             *(float *)((char *)this + 0x78);
                if (area > 0.0f) {
                    *(unsigned char *)((char *)this + 0x8C) =
                        (unsigned char)(*(unsigned char *)((char *)this + 0x8C) | 4);
                    if (*(void **)((char *)this + 0x64) != 0) {
                        if (*(void **)((char *)this + 0x80) == 0
                            || *(void **)((char *)this + 0x84) == 0) {
                            eGeom *head = s_pFirstUpdate;
                            if (head != 0) {
                                *(void **)((char *)this + 0x80) = *(void **)((char *)head + 0x80);
                                *(eGeom **)((char *)this + 0x84) = s_pFirstUpdate;
                                *(eGeom **)((char *)*(void **)((char *)this + 0x80) + 0x84) = this;
                                *(eGeom **)((char *)*(void **)((char *)this + 0x84) + 0x80) = this;
                            } else {
                                s_pFirstUpdate = this;
                                *(eGeom **)((char *)this + 0x84) = this;
                                *(eGeom **)((char *)this + 0x80) = this;
                            }
                        }
                    }
                    int *entry = (int *)((char *)*(int **)((char *)this + 4) + 0xB8);
                    short adj = *(short *)entry;
                    void (*fn)(void *) = (void (*)(void *))entry[1];
                    fn((char *)this + adj);
                    goto done;
                }
            }

            eRoom *room = *(eRoom **)((char *)this + 0x68);
            if (room != 0) {
                room->RemoveGeom(this);
            } else {
                eWorld *world = *(eWorld **)((char *)this + 0x64);
                if (world != 0 && *(int *)((char *)world + 0x24) == 0) {
                    if (*(unsigned char *)((char *)this + 0x8C) & 0x10) {
                        world->RemoveBinGeom(this, eWorld::eWorldGeomBin_Decal);
                    } else if (this != 0 && *(void **)((char *)this + 0x6C) != 0) {
                        if (*(void **)((char *)this + 0x70) != 0) {
                            if (*(eGeom **)world == this) {
                                *(void **)world = *(void **)((char *)this + 0x70);
                            }
                            *(void **)((char *)*(void **)((char *)this + 0x6C) + 0x70) =
                                *(void **)((char *)this + 0x70);
                            *(void **)((char *)*(void **)((char *)this + 0x70) + 0x6C) =
                                *(void **)((char *)this + 0x6C);
                            *(void **)((char *)this + 0x6C) = 0;
                            *(void **)((char *)this + 0x70) = 0;
                            if (*(eGeom **)world == this) {
                                *(eGeom **)world = 0;
                            }
                        }
                    }
                }
            }

            if (*(unsigned char *)((char *)this + 0x8C) & 4) {
                if ((*(void **)((char *)this + 0x80) != 0
                     || *(void **)((char *)this + 0x84) != 0)
                    && this != 0 && *(void **)((char *)this + 0x80) != 0) {
                    void *prev = *(void **)((char *)this + 0x84);
                    if (prev != 0) {
                        if (s_pFirstUpdate == this) {
                            s_pFirstUpdate = (eGeom *)prev;
                        }
                        *(void **)((char *)*(void **)((char *)this + 0x80) + 0x84) =
                            *(void **)((char *)this + 0x84);
                        *(void **)((char *)*(void **)((char *)this + 0x84) + 0x80) =
                            *(void **)((char *)this + 0x80);
                        *(void **)((char *)this + 0x80) = 0;
                        *(void **)((char *)this + 0x84) = 0;
                        if (s_pFirstUpdate == this) {
                            s_pFirstUpdate = 0;
                        }
                    }
                }
            }
            *(float *)((char *)this + 0x5C) = -1.0f;
        }
    }
done:
    ;
}
