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
class cType;

// ── cWriteBlock helper (matches eDynamicGeom.cpp / cFactory.cpp shape) ──
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

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046C20;

#pragma control sched=1

class eDynamicGeom {
public:
    char _pad[0xEC];
    eDynamicGeom(cBase *);
    ~eDynamicGeom();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eGeomCurve : public eDynamicGeom {
public:
    eGeomCurve(cBase *);
    ~eGeomCurve();
    eGeomCurve &operator=(const eGeomCurve &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
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

// ── eGeomCurve::Read(cFile &, cMemPool *) @ 0x00078364 ──
int eGeomCurve::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
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
cBase *eGeomCurve::New(cMemPool *pool, cBase *parent) {
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
    return (cBase *)result;
}

const cType *eGeomCurve::GetType(void) const {
    if (D_00046C20 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))&eGeomCurve::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C20 = cType::InitializeType(0, 0, 0x1D4, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C20;
}
#pragma control sched=2

// ── eGeomCurve::AssignCopy(const cBase *) @ 0x00210e5c ──
void eGeomCurve::AssignCopy(const cBase *base) {
    __asm__ volatile("" ::: "memory");
    const eGeomCurve *other = 0;

    if (base != 0) {
        if (D_00046C20 == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        __asm__ volatile("" : "+r"(name));
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                       0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                                   0, 0, 0, 0);
            }
            __asm__ volatile("" ::: "memory");
            const cType *parentType = D_000469C0;
            __asm__ volatile("" : "+r"(parentType));
            cBase *(*factory)(cMemPool *, cBase *) = &eGeomCurve::New;
            __asm__ volatile("" : "+r"(factory));
            D_00046C20 = cType::InitializeType(0, 0, 0x1D4, parentType, factory,
                                               0, 0, 0);
        }

        cType *target = D_00046C20;
        __asm__ volatile("" : "+r"(target));
        void *classDesc = *(void **)((const char *)base + 4);
        void *entryVoid = (char *)classDesc + 8;
        DispatchEntry *entry = (DispatchEntry *)entryVoid;
        cType *(*fn)(void *);
        short offset = entry->offset;
        fn = entry->fn;
        cType *type = fn((void *)((const char *)base + offset));
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = *(cType **)((char *)type + 0x1C);
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const eGeomCurve *)base;
        }
    }

    *this = *other;
}
