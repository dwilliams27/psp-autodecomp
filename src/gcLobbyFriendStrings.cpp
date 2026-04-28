// gcLobbyFriendStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyFriendStrings
//   Total size 0x10. Layout:
//     0x00: cBase (mOwner, mClassDesc)
//     0x08: gcDesiredValue (tagged self-pointer)
//     0x0C: int mField

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcLobbyFriendStringsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLobbyFriendStrings : public gcStringValue {
public:
    int _b8;   // 0x08 - gcDesiredValue tagged self-ptr
    int mField; // 0x0C

    void Write(cFile &) const;
    static gcLobbyFriendStrings *New(cMemPool *, cBase *);
};

// ── gcLobbyFriendStrings::Write(cFile &) const  @ 0x00280c18, 100B ──
void gcLobbyFriendStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(this->mField);
    wb.End();
}

// ── gcLobbyFriendStrings::New(cMemPool *, cBase *) static  @ 0x00280aac, 144B ──
gcLobbyFriendStrings *gcLobbyFriendStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyFriendStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4) = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4) = (int)gcLobbyFriendStringsvirtualtable;
        *(int *)((char *)p + 8) = (int)((unsigned int)p | 1);
        *(int *)((char *)p + 12) = 0;
        result = (gcLobbyFriendStrings *)p;
    }
    return result;
}
