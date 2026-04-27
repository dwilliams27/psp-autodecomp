// gcValLobbyConnectionStatus — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x003478F8  AssignCopy(const cBase *)
//   0x00347928  New(cMemPool *, cBase *) static
//   0x00347ACC  Write(cFile &) const
//   0x00347B24  Read(cFile &, cMemPool *)
//
// Class layout (12 bytes, alloc size 0xC):
//   [0x00] parent (cBase *)
//   [0x04] vtable
//   [0x08] f8 (lobby-connection-status int, defaults to 1 in New)

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
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValLobbyConnectionStatus : public gcValue {
public:
    int pad0;
    int f8;
    int pad4;

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

gcValLobbyConnectionStatus *dcast(const cBase *);

extern char gcValLobbyConnectionStatusvirtualtable[];
extern char gcValLobbyConnectionStatus_cBase_vtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValLobbyConnectionStatus::AssignCopy(const cBase *) @ 0x003478F8 ──
void gcValLobbyConnectionStatus::AssignCopy(const cBase *base) {
    gcValLobbyConnectionStatus *other = dcast(base);
    this->f8 = other->f8;
}

// ── gcValLobbyConnectionStatus::New(cMemPool *, cBase *) static @ 0x00347928 ──
cBase *gcValLobbyConnectionStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyConnectionStatus *result = 0;
    gcValLobbyConnectionStatus *obj =
        (gcValLobbyConnectionStatus *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValLobbyConnectionStatus_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbyConnectionStatusvirtualtable;
        ((int *)obj)[2] = 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyConnectionStatus::Write(cFile &) const @ 0x00347ACC ──
void gcValLobbyConnectionStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->f8);
    wb.End();
}

// ── gcValLobbyConnectionStatus::Read(cFile &, cMemPool *) @ 0x00347B24 ──
int gcValLobbyConnectionStatus::Read(cFile &file, cMemPool *pool) {
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
