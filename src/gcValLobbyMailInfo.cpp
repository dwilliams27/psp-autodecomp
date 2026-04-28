// gcValLobbyMailInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034a140  Write(cFile &) const
//
// Class layout (partial):
//   [0x00] gcValue base (parent + vtable)
//   [0x08] embedded gcDesiredValue
//   [0x0C] mField (int written between base and gcDesiredValue::Write)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cObject;

extern char cBaseclassdesc[];
extern char gcValLobbyMailInfovirtualtable[];
extern char gcValObjectComparevirtualtable[];
extern char eSurfacePropertyTablevirtualtable[];

struct AllocBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValLobbyMailInfo : public gcValue {
public:
    int mField8;   // 0x08 — start of embedded gcDesiredValue
    int mFieldC;   // 0x0C — int field written by wb.Write before gcDesiredValue::Write

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcValObjectCompare : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject(cBase *);
};

extern "C" void cObject_ctor(void *, cBase *) __asm__("__0oHcObjectctP6FcBase");

class eSurfacePropertyTable {
public:
    static cBase *New(cMemPool *, cBase *);
};

// ── gcValLobbyMailInfo::New(cMemPool *, cBase *) static @ 0x00349f98 ──
cBase *gcValLobbyMailInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyMailInfo *result = 0;
    gcValLobbyMailInfo *obj = (gcValLobbyMailInfo *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyMailInfovirtualtable;
        *(int *)((char *)obj + 8) = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValObjectCompare::New(cMemPool *, cBase *) static @ 0x00353978 ──
cBase *gcValObjectCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectCompare *result = 0;
    gcValObjectCompare *obj = (gcValObjectCompare *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValObjectComparevirtualtable;
        int flagged = (int)obj | 1;
        *(int *)((char *)obj + 8) = flagged;
        *(int *)((char *)obj + 0xC) = flagged;
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1

// ── eSurfacePropertyTable::New(cMemPool *, cBase *) static @ 0x001f6f80 ──
cBase *eSurfacePropertyTable::New(cMemPool *pool, cBase *parent) {
    eSurfacePropertyTable *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eSurfacePropertyTable *obj = (eSurfacePropertyTable *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        cObject_ctor(obj, parent);
        *(void **)((char *)obj + 4) = eSurfacePropertyTablevirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        *(eSurfacePropertyTable **)((char *)obj + 0x48) = obj;
        *(int *)((char *)obj + 0x4C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2

// ── gcValLobbyMailInfo::Write(cFile &) const @ 0x0034a140 ──
void gcValLobbyMailInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mFieldC);
    ((gcDesiredValue *)((char *)this + 8))->Write(wb);
    wb.End();
}
