// gcValNetworkState — decompiled from gcAll_psp.obj.
// Methods in this file:
//   gcValNetworkState::New(cMemPool *, cBase *) static  @ 0x00352db4 (144B)
//   gcValNetworkState::Write(cFile &) const             @ 0x00352f5c (100B)
//
// Class layout (16 bytes, alloc size 0x10):
//   [0x00] parent (cBase *)
//   [0x04] vtable
//   [0x08] mField8 (int, defaults to 0 in New)
//   [0x0C] gcDesiredValue (4 bytes; init'd as (this|1))

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
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

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcValNetworkStatevirtualtable[];
extern char gcValNetworkState_cBase_vtable[];

class gcValNetworkState : public gcValue {
public:
    int pad0;
    int pad4;
    int mField8;
    int mField0C;

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

// ── gcValNetworkState::Write(cFile &) const @ 0x00352f5c ──
void gcValNetworkState::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mField8);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.End();
}

// ── gcValNetworkState::New(cMemPool *, cBase *) static @ 0x00352db4 ──
cBase *gcValNetworkState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValNetworkState *result = 0;
    gcValNetworkState *obj =
        (gcValNetworkState *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValNetworkState_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValNetworkStatevirtualtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}
