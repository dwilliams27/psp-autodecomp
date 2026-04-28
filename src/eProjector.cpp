// eProjector.cpp — eProjector engine class
// Base: eDynamicGeom : eGeom : cObject
// Vtable symbol: eProjectorvirtualtable (shared layout with template)
#pragma control sched=1

class cBase;
class cFile;
class cMemPool_fwd;

class cTimeValue {
public:
    float mTime;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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
    char _dynPad[0x58];
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

class eProjector : public eDynamicGeom {
public:
    eProjector(cBase *);
    ~eProjector();
    void Write(cFile &) const;
    void Reset(cMemPool *, bool);
    void Update(cTimeValue);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        char *allocTable = *(char **)((char *)block + 0x1C);
        DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        void *base = (char *)block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char eProjectorvirtualtable[];

// ── Constructor ──
eProjector::eProjector(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eProjectorvirtualtable;
}

// ── Destructor ──
eProjector::~eProjector() {
    *(void **)((char *)this + 4) = eProjectorvirtualtable;
}

// ── New ──
cBase *eProjector::New(cMemPool *pool, cBase *parent) {
    eProjector *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eProjector *obj = (eProjector *)rec->fn(base, 0xF0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eProjector(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── Reset ──
void eProjector::Reset(cMemPool *, bool) {
    *(unsigned char *)((char *)this + 0x8D) = 1;
    __asm__ volatile("" ::: "memory");
    void *p = *(void **)((char *)this + 0x60);
    if (p != 0) {
        *(float *)((char *)this + 0x74) = *(float *)((char *)p + 0x58);
    } else {
        *(float *)((char *)this + 0x74) = -1.0f;
    }
}

// ── Update ──
void eProjector::Update(cTimeValue) {
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}

// ── Write ──
void eProjector::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eDynamicGeom::Write(file);
    wb.End();
}
