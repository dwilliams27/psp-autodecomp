// gcValLobbyGameInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x00348c28  New(cMemPool *, cBase *) static
//   0x00348dd8  Write(cFile &) const
//
// Class layout (20 bytes, alloc size 0x14):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mField8 (int — written by Write between gcValue::Write and the gcDesiredValue::Write)
//   [0x0C] embedded gcDesiredValue (4 bytes; self-init to (this | 1) in New)
//   [0x10] mField10 (int — defaults to 5 in New)

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

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcValuevirtualtable[];
extern char gcValLobbyGameInfovirtualtable[];

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    gcValue(cBase *parent);
    void Write(cFile &) const;
};

class gcValLobbyGameInfo : public gcValue {
public:
    int mField8;
    int mFieldC;       // embedded gcDesiredValue (single self-pointer field)
    int mField10;

    gcValLobbyGameInfo(cBase *parent);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

inline gcValue::gcValue(cBase *parent) {
    mVtable = gcValuevirtualtable;
    mParent = parent;
}

inline gcValLobbyGameInfo::gcValLobbyGameInfo(cBase *parent) : gcValue(parent) {
    mVtable = gcValLobbyGameInfovirtualtable;
    mField8 = 0;
    mFieldC = (int)this | 1;
    mField10 = 5;
}

inline void *operator new(unsigned, void *p) { return p; }

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValLobbyGameInfo::New(cMemPool *, cBase *) static @ 0x00348c28 ──
cBase *gcValLobbyGameInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValLobbyGameInfo *result = 0;
    gcValLobbyGameInfo *obj = (gcValLobbyGameInfo *)e->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValLobbyGameInfo(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyGameInfo::Write(cFile &) const @ 0x00348dd8 ──
void gcValLobbyGameInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mField10);
    ((gcDesiredValue *)((char *)this + 12))->Write(wb);
    wb.End();
}
