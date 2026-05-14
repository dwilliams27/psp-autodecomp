// eGeomTrail::eGeomTrail(cBase *) @ 0x00079460
// Inherits eDynamicGeom : eGeom : cObject. In sched=1 zone.
#pragma control sched=1

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    ~eDynamicGeom();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _dynPad[0x58];
};

template <class T>
class cArrayBase {
public:
    bool SetSize(int, const cArrayBase<T> &);
};

struct DeleteEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eGeomTrailParticle;

struct eGeomTrailParticleDefault {
    int words[8];
};

struct eGeomTrailHandleEntry {
    char _pad0[0x30];
    int handle;
    char _pad34[0x29];
    unsigned char flag;
};

class eGeomTrail : public eDynamicGeom {
public:
    eGeomTrail(cBase *);
    ~eGeomTrail();
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }

    const cType *GetType(void) const;
    void Reset(cMemPool *, bool);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _trailPad[0x30];
};

extern eGeomTrailHandleEntry *D_00038890[];
extern char eGeomTrailvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_00046C2C;

extern "C" {
    void eGeomTrail__eGeomTrail_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

eGeomTrail::eGeomTrail(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eGeomTrailvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0xF8) = 0;
    *(int *)((char *)this + 0xFC) = 0;
}

// ── eGeomTrail::~eGeomTrail(void) @ 0x000794A0 ──
eGeomTrail::~eGeomTrail() {
    *(void **)((char *)this + 4) = eGeomTrailvirtualtable;
    void *field = (char *)this + 0xF8;
    if (field != 0) {
        void *entries = *(void **)((char *)this + 0xF8);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0xF8) = 0;
        }
    }
}

// ── eGeomTrail::Reset(cMemPool *, bool) @ 0x000795C4 ──
void eGeomTrail::Reset(cMemPool *, bool) {
    int stackPad[4];
    stackPad[0] = 0;
    __asm__ volatile("" : "+m"(stackPad));

    int *tailCount = (int *)((char *)this + 0xFC);
    *tailCount = 0;
    *(int *)((char *)this + 0xF4) = 0;

    cArrayBase<eGeomTrailParticle> *particles =
        (cArrayBase<eGeomTrailParticle> *)((char *)this + 0xF8);
    void *templCheck = *(void **)((char *)this + 0x60);
    if (templCheck == 0) {
        goto no_template;
    }

    void *templ = templCheck;
    {
        int size = 0;
        __asm__ volatile("" : "+r"(size));
        size = *(int *)((char *)templ + 0x50) + 1;
        eGeomTrailParticleDefault def;
        def.words[0] = 0;
        eGeomTrailParticleDefault *defPtr = &def;
        __asm__ volatile("" : "+r"(defPtr));
        defPtr->words[1] = 0;
        defPtr->words[2] = 0;
        defPtr->words[3] = 0;
        defPtr->words[4] = 0;
        defPtr->words[5] = 0;
        defPtr->words[6] = 0;
        defPtr->words[7] = 0;
        particles->SetSize(size, *(cArrayBase<eGeomTrailParticle> *)defPtr);
    }

    *(unsigned char *)((char *)this + 0x8D) = 0;
    register int handle __asm__("$4") = *(int *)((char *)templ + 0x48);
    eGeomTrailHandleEntry *valid;
    if (handle != 0) {
        goto first_lookup;
    }
    valid = 0;
    goto first_done;

no_template:
    {
        eGeomTrailParticleDefault def;
        def.words[0] = 0;
        eGeomTrailParticleDefault *defPtr = &def;
        __asm__ volatile("" : "+r"(defPtr));
        defPtr->words[1] = 0;
        defPtr->words[2] = 0;
        defPtr->words[3] = 0;
        defPtr->words[4] = 0;
        defPtr->words[5] = 0;
        defPtr->words[6] = 0;
        defPtr->words[7] = 0;
        particles->SetSize(0, *(cArrayBase<eGeomTrailParticle> *)defPtr);
        return;
    }

first_lookup:
    {
        eGeomTrailHandleEntry *candidate = D_00038890[handle & 0xFFFF];
        valid = 0;
        if (candidate != 0) {
            if (candidate->handle == handle) {
                valid = candidate;
            }
        }
    }

first_done:
    if (valid != 0) {
        eGeomTrailHandleEntry *entry;
        unsigned char flag;
        if (handle == 0) {
            entry = 0;
            flag = entry->flag;
            goto have_flag;
        } else {
            eGeomTrailHandleEntry *candidate = D_00038890[handle & 0xFFFF];
            entry = 0;
            if (candidate != 0) {
                if (candidate->handle == handle) {
                    entry = candidate;
                }
            }
        }
        flag = entry->flag;
have_flag:
        *(unsigned char *)((char *)this + 0x8D) = flag;
    }
}

// ── eGeomTrail::Write(cFile &) const @ 0x00079358 ──
void eGeomTrail::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eGeomTrail::Read(cFile &, cMemPool *) @ 0x000793A4 ──
int eGeomTrail::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eGeomTrail::New(cMemPool *, cBase *) static @ 0x00211CB4 ──
cBase *eGeomTrail::New(cMemPool *pool, cBase *parent) {
    eGeomTrail *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomTrail *obj = (eGeomTrail *)rec->fn(base, 0x100, 0x10, 0, 0);
    if (obj != 0) {
        eGeomTrail__eGeomTrail_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eGeomTrail::GetType(void) const @ 0x00211D30 ──
const cType *eGeomTrail::GetType(void) const {
    if (D_00046C2C == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = eGeomTrail::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C2C = cType::InitializeType(0, 0, 0x34, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C2C;
}
