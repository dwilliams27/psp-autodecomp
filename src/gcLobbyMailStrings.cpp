// gcLobbyMailStrings — string-table value bound to a desired-value lvalue.
//
// Functions matched here:
//   gcLobbyMailStrings::Write(cFile &) const            @ 0x002821f8  (gcAll_psp.obj)
//   gcLobbyMailStrings::New(cMemPool *, cBase *) static @ 0x0028208c  (gcAll_psp.obj)

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
extern char gcLobbyMailStringsclassdesc[];

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

class gcLobbyMailStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — gcDesiredValue first slot, holds (this | 1)
    int mField0C;       // 0x0C

    gcLobbyMailStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyMailStringsclassdesc;
        mField08 = (int)this | 1;
        mField0C = 0;
    }
    void Write(cFile &) const;
    static gcLobbyMailStrings *New(cMemPool *, cBase *);
};

// ── Write ──  @ 0x002821f8, 100B
void gcLobbyMailStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(mField0C);
    wb.End();
}

// ── New ──  @ 0x0028208c, 144B
gcLobbyMailStrings *gcLobbyMailStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyMailStrings *result = 0;
    gcLobbyMailStrings *obj = (gcLobbyMailStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyMailStrings(parent);
        result = obj;
    }
    return result;
}
