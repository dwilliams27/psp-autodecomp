// gcLobbyUserStrings — string-table value bound to a desired-value lvalue.
//
// Functions matched here:
//   gcLobbyUserStrings::Write(cFile &) const            @ 0x00284888  (gcAll_psp.obj)
//   gcLobbyUserStrings::New(cMemPool *, cBase *) static @ 0x0028471c  (gcAll_psp.obj)

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
extern char gcLobbyUserStringsclassdesc[];

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

class gcLobbyUserStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — gcDesiredValue first slot, holds (this | 1)
    int mField0C;       // 0x0C

    gcLobbyUserStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyUserStringsclassdesc;
        mField08 = (int)this | 1;
        mField0C = 0;
    }
    void Write(cFile &) const;
    static gcLobbyUserStrings *New(cMemPool *, cBase *);
};

// ── Write ──  @ 0x00284888, 100B
void gcLobbyUserStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(mField0C);
    wb.End();
}

// ── New ──  @ 0x0028471c, 144B
gcLobbyUserStrings *gcLobbyUserStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyUserStrings *result = 0;
    gcLobbyUserStrings *obj = (gcLobbyUserStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyUserStrings(parent);
        result = obj;
    }
    return result;
}
