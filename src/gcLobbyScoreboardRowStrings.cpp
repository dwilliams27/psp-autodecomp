// gcLobbyScoreboardRowStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyScoreboardRowStrings
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
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

extern char gcLobbyScoreboardRowStringsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLobbyScoreboardRowStrings : public gcStringValue {
public:
    int _b8;    // 0x08 - gcDesiredValue tagged self-ptr
    int mField; // 0x0C

    const cType *GetType(void) const;
    void Write(cFile &) const;
    static gcLobbyScoreboardRowStrings *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F500;

// ── gcLobbyScoreboardRowStrings::GetType(void) const  @ 0x00283d68, 220B ──
const cType *gcLobbyScoreboardRowStrings::GetType(void) const {
    if (D_0009F500 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F500 = cType::InitializeType(
            0, 0, 0x128, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyScoreboardRowStrings::New,
            0, 0, 0);
    }
    return D_0009F500;
}

// ── gcLobbyScoreboardRowStrings::Write(cFile &) const  @ 0x00283e44, 100B ──
void gcLobbyScoreboardRowStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    wb.Write(this->mField);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// ── gcLobbyScoreboardRowStrings::New(cMemPool *, cBase *) static  @ 0x00283cd8, 144B ──
gcLobbyScoreboardRowStrings *gcLobbyScoreboardRowStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyScoreboardRowStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4) = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4) = (int)gcLobbyScoreboardRowStringsvirtualtable;
        *(int *)((char *)p + 8) = (int)((unsigned int)p | 1);
        *(int *)((char *)p + 12) = 0;
        result = (gcLobbyScoreboardRowStrings *)p;
    }
    return result;
}
