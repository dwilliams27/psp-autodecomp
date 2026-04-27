#include "eCameraEffectLayer.h"
#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

template <class T> T *dcast(const cBase *);

extern const char eCameraEffectLayer_base_name[];
extern const char eCameraEffectLayer_base_desc[];

static cType *type_base;
static cType *type_eCameraEffectLayer;

extern char cBaseclassdesc[];                       // @ 0x37E6A8
extern char eCameraEffectLayervirtualtable[];       // @ 0x3800C8

struct ePoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct eDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eMemPoolNS {
public:
    static eMemPoolNS *GetPoolFromPtr(const void *);
};

// AssignCopy (0x001e2808)
void eCameraEffectLayer::AssignCopy(const cBase *other) {
    eCameraEffectLayer *p = dcast<eCameraEffectLayer>(other);
    mField8 = p->mField8;
    mFieldC = p->mFieldC;
    mField10 = p->mField10;
}

// Write (0x0002cfec)
void eCameraEffectLayer::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    wb.Write(mFieldC);
    mField10.Write(wb);
    wb.Write(mField8);
    wb.End();
}

// operator delete (inlined into ~eCameraEffectLayer deleting variant)
inline void eCameraEffectLayer::operator delete(void *p) {
    eMemPoolNS *pool = eMemPoolNS::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    eDeleteRecord *r = (eDeleteRecord *)(((ePoolBlock *)block)->allocTable + 0x30);
    short off = r->offset;
    void (*fn)(void *, void *) = r->fn;
    fn(block + off, p);
}

// ~eCameraEffectLayer (0x001e2c9c)
eCameraEffectLayer::~eCameraEffectLayer() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// New (0x001e2850)
cBase *eCameraEffectLayer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eCameraEffectLayer *result = 0;
    eCameraEffectLayer *obj = (eCameraEffectLayer *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = eCameraEffectLayervirtualtable;
        *(char *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = 3;
        *(int *)((char *)obj + 0x10) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// GetType (0x001e28e4)
const cType *eCameraEffectLayer::GetType(void) const {
    if (!type_eCameraEffectLayer) {
        if (!type_base) {
            type_base = cType::InitializeType(eCameraEffectLayer_base_name, eCameraEffectLayer_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_eCameraEffectLayer = cType::InitializeType(0, 0, 0x2B, type_base, &eCameraEffectLayer::New, 0, 0, 0);
    }
    return type_eCameraEffectLayer;
}

// ──────────────────────────────────────────────────────────────────────────
// eSurfaceProperty::~eSurfaceProperty(void)  @ 0x001f6170
// ──────────────────────────────────────────────────────────────────────────

class eSurfaceProperty {
public:
    ~eSurfaceProperty();
    static void operator delete(void *p) {
        eMemPoolNS *pool = eMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        eDeleteRecord *r = (eDeleteRecord *)(((ePoolBlock *)block)->allocTable + 0x30);
        short off = r->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = r->fn;
        fn(base, p);
    }
};

eSurfaceProperty::~eSurfaceProperty() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
