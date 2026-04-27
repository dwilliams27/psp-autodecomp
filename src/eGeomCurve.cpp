// eGeomCurve methods (eAll_psp.obj — sched=1 zone)
//
// Three functions: Write(cFile&), ~eGeomCurve(), New(cMemPool*, cBase*).
// Patterns adapted from eDynamicGeom.cpp (Write), eSphereShape_dtor.cpp (dtor),
// and eConfigBase.cpp (New).
//
// Local class redeclarations are used (mirroring eSphereShape_dtor.cpp) so we
// can declare eGeomCurve : eDynamicGeom with the right ABI shape and put the
// `operator delete` body inline for the deleting-tail dispatch.

class cBase;
class cFile;
class cMemPool;

// ── cWriteBlock helper (matches eDynamicGeom.cpp / cFactory.cpp shape) ──
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

// Vtable symbol — `lui 0x38; addiu 17592` => 0x003844B8.
extern char eGeomCurvevirtualtable[];

// Pool-layout helpers (match eSphereShape_dtor.cpp / eConfigBase.cpp style).
struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

#pragma control sched=1

class eDynamicGeom {
public:
    char _pad[0xEC];
    eDynamicGeom(cBase *);
    ~eDynamicGeom();
    void Write(cFile &) const;
};

class eGeomCurve : public eDynamicGeom {
public:
    eGeomCurve(cBase *);
    ~eGeomCurve();
    void Write(cFile &) const;
    static eGeomCurve *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ── eGeomCurve::Write(cFile &) const @ 0x00078318 ──
void eGeomCurve::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eDynamicGeom *)this)->Write(file);
    wb.End();
}

// ── eGeomCurve::~eGeomCurve(void) @ 0x000786b8 ──
eGeomCurve::~eGeomCurve() {
    *(void **)((char *)this + 4) = eGeomCurvevirtualtable;
}

#pragma control sched=2

// ── eGeomCurve::New(cMemPool *, cBase *) static @ 0x00210ffc ──
//
// Forward-declare the existing constructor symbol so we can invoke it as a
// plain call after the alloc returns the raw memory (same idiom as
// eConfigBase::New). This is *not* a method-definition wrapper — the actual
// definition of eGeomCurve::eGeomCurve(cBase*) lives in the bodied ctor file
// at 0x78420.
extern "C" void eGeomCurve__eGeomCurve_cBaseptr(void *self, cBase *parent);

#pragma control sched=1
eGeomCurve *eGeomCurve::New(cMemPool *pool, cBase *parent) {
    eGeomCurve *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomCurve *obj = (eGeomCurve *)rec->fn(base, 0x250, 0x10, 0, 0);
    if (obj != 0) {
        eGeomCurve__eGeomCurve_cBaseptr(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
