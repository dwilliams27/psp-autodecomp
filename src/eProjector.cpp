// eProjector.cpp — eProjector engine class
// Base: eDynamicGeom : eGeom : cObject
// Vtable symbol: eProjectorvirtualtable (shared layout with template)
#pragma control sched=1

class cBase;
class cFile;
class cMemPool_fwd;

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    void Write(cFile &) const;
    char _dynPad[0x58];
};

class eProjector : public eDynamicGeom {
public:
    eProjector(cBase *);
    void Write(cFile &) const;
    void Reset(cMemPool *, bool);
    void Update(int /*cTimeValue*/);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char eProjectorvirtualtable[];

// Extern C-style destructors (SNC direct symbol calls)
extern "C" void eDynamicGeom___dtor_eDynamicGeom_void(void *, int);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// ── Constructor ──
eProjector::eProjector(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eProjectorvirtualtable;
}

// ── Destructor ──
extern "C" {

void eProjector___dtor_eProjector_void(eProjector *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eProjectorvirtualtable;
        eDynamicGeom___dtor_eDynamicGeom_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}

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
void eProjector::Update(int /*tv*/) {
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
