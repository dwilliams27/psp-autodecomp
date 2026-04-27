// gcValLobbySessionStatus — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034e4fc  New(cMemPool *, cBase *) static
//   0x0034e69c  Write(cFile &) const
//   0x0034e6f4  Read(cFile &, cMemPool *)
//
// Class layout (12 bytes, alloc size 0xC):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mField8 (defaults to 0 in New)

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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern char gcValLobbySessionStatus_gcValue_vtable[];
extern char gcValLobbySessionStatusvirtualtable[];

class gcValLobbySessionStatus : public gcValue {
public:
    int mField8;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
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

// ── gcValLobbySessionStatus::New(cMemPool *, cBase *) static @ 0x0034e4fc ──
cBase *gcValLobbySessionStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbySessionStatus *result = 0;
    gcValLobbySessionStatus *obj =
        (gcValLobbySessionStatus *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValLobbySessionStatus_gcValue_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbySessionStatusvirtualtable;
        ((int *)obj)[2] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbySessionStatus::Write(cFile &) const @ 0x0034e69c ──
void gcValLobbySessionStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mField8);
    wb.End();
}

// ── gcValLobbySessionStatus::Read(cFile &, cMemPool *) @ 0x0034e6f4 ──
int gcValLobbySessionStatus::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 8, 4);
    }
    return result;
}
