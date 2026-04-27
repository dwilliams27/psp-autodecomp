// gcVoiceOver.cpp — decompiled from gcAll_psp.obj.
// Parent: eSoundData. Vtable at 0x388C40.
//
// Functions:
//   0x001211c4 gcVoiceOver::Write(cFile &) const                100B
//   0x00121414 gcVoiceOver::~gcVoiceOver(void)                  124B
//   0x00262594 gcVoiceOver::New(cMemPool *, cBase *) static     124B
//   (already matched)
//   gcVoiceOver::IsVoiceOver(void) const                          8B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool_shim {
public:
    static cMemPool_shim *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class eSoundData {
public:
    char _pad[0x68];
    eSoundData(cBase *);
    ~eSoundData(void);
    void Write(cFile &) const;
};

class gcVoiceOver : public eSoundData {
public:
    cHandle mHandle;        // 0x68
    unsigned int mField6C;  // 0x6C

    gcVoiceOver(cBase *);
    ~gcVoiceOver(void);
    int IsVoiceOver(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool_shim *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcVoiceOvervirtualtable[];

// ── gcVoiceOver::IsVoiceOver(void) const ──
int gcVoiceOver::IsVoiceOver(void) const {
    return 1;
}

// ── gcVoiceOver::Write @ 0x001211c4 ──
void gcVoiceOver::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    eSoundData::Write(file);
    mHandle.Write(wb);
    wb.Write(mField6C);
    wb.End();
}

// ── gcVoiceOver::~gcVoiceOver @ 0x00121414 ──
gcVoiceOver::~gcVoiceOver(void) {
    *(void **)((char *)this + 4) = gcVoiceOvervirtualtable;
}

// ── gcVoiceOver::New @ 0x00262594 ──
cBase *gcVoiceOver::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcVoiceOver *result = 0;
    gcVoiceOver *obj = (gcVoiceOver *)rec->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcVoiceOver(parent);
        result = obj;
    }
    return (cBase *)result;
}
