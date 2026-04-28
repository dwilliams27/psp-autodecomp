// gcLobbyAccountStrings — string-table value with extra fixed flag.
//
// Functions matched here:
//   gcLobbyAccountStrings::Write(cFile &) const            @ 0x0027fcc4  (gcAll_psp.obj)
//   gcLobbyAccountStrings::New(cMemPool *, cBase *) static @ 0x0027fb50  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

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

extern char cBaseclassdesc[];
extern char gcLobbyAccountStringsclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cBase {
public:
    cBase *mOwner;          // 0
    void *mClassDesc;       // 4

    cBase(cBase *owner) {
        mClassDesc = cBaseclassdesc;
        mOwner = owner;
    }
};

class gcStringValue : public cBase {
public:
    gcStringValue(cBase *parent) : cBase(parent) {}
    void Write(cFile &) const;
};

class gcLobbyAccountStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — constant 1
    int mField0C;       // 0x0C — gcDesiredValue first slot, holds (this | 1)
    int mField10;       // 0x10

    gcLobbyAccountStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyAccountStringsclassdesc;
        mField08 = 1;
        mField0C = (int)this | 1;
        mField10 = 0;
    }
    void Write(cFile &) const;
    static gcLobbyAccountStrings *New(cMemPool *, cBase *);
};

// ── Write ──  @ 0x0027fcc4, 112B
void gcLobbyAccountStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringValue::Write(file);
    wb.Write(mField08);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mField10);
    wb.End();
}

// ── New ──  @ 0x0027fb50, 152B
gcLobbyAccountStrings *gcLobbyAccountStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyAccountStrings *result = 0;
    gcLobbyAccountStrings *obj = (gcLobbyAccountStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyAccountStrings(parent);
        result = obj;
    }
    return result;
}
