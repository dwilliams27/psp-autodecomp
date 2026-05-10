// eGeom::~eGeom(void) @ 0x0002c5c0, 440B
//
// Split TU per CLAUDE.md SPLIT-TU rule. The rest of eGeom's matched methods
// live in src/eGeom.cpp; adding a destructor declaration to that file's class
// could shift codegen for matched siblings (Write, UpdateLocalToWorld, GetType,
// CastRay, CastSphere).
//
// ODR-WARNING: eGeom is also defined in src/eGeom.cpp. The two TUs reference
// disjoint method sets and agree on the field offsets touched here.

class eGeom;
class eRoom;
class eWorld;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
}

class eRoom {
public:
    void RemoveGeom(eGeom *);
};

class eWorld {
public:
    enum eWorldGeomBin {
        eWorldGeomBin_Decal = 2
    };
    void RemoveBinGeom(eGeom *, eWorldGeomBin);
};

class eGeom {
public:
    static eGeom *s_pFirstUpdate;
    ~eGeom();
};

__asm__(".size __0oFeGeomdtv, 0x1b8\n");

eGeom::~eGeom() {
    *(void **)((char *)this + 4) = (void *)0x37FF48;

    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        if ((*(void **)((char *)this + 0x80) != 0
             || *(void **)((char *)this + 0x84) != 0)
            && this != 0 && *(void **)((char *)this + 0x80) != 0) {
            if (*(eGeom **)((char *)this + 0x84) != 0) {
                if (eGeom::s_pFirstUpdate == this) {
                    eGeom::s_pFirstUpdate = *(eGeom **)((char *)this + 0x84);
                }
                *(eGeom **)((char *)*(void **)((char *)this + 0x80) + 0x84) =
                    *(eGeom **)((char *)this + 0x84);
                *(eGeom **)((char *)*(void **)((char *)this + 0x84) + 0x80) =
                    *(eGeom **)((char *)this + 0x80);
                *(void **)((char *)this + 0x80) = 0;
                *(void **)((char *)this + 0x84) = 0;
                if (eGeom::s_pFirstUpdate == this) {
                    eGeom::s_pFirstUpdate = 0;
                }
            }
        }
    }

    if (*(eRoom **)((char *)this + 0x68) != 0) {
        (*(eRoom **)((char *)this + 0x68))->RemoveGeom(this);
    } else if (*(eWorld **)((char *)this + 0x64) != 0
               && *(int *)((char *)*(eWorld **)((char *)this + 0x64) + 0x24) == 0) {
        if (*(unsigned char *)((char *)this + 0x8C) & 0x10) {
            (*(eWorld **)((char *)this + 0x64))
                ->RemoveBinGeom(this, eWorld::eWorldGeomBin_Decal);
        } else if (*(void **)((char *)this + 0x6C) != 0) {
            if (*(void **)((char *)this + 0x70) != 0) {
                eWorld *world = *(eWorld **)((char *)this + 0x64);
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

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
