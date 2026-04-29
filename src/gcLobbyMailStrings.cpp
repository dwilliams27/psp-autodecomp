// gcLobbyMailStrings — string-table value bound to a desired-value lvalue.
//
// Functions matched here:
//   gcLobbyMailStrings::Write(cFile &) const            @ 0x002821f8  (gcAll_psp.obj)
//   gcLobbyMailStrings::New(cMemPool *, cBase *) static @ 0x0028208c  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cType;

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static gcLobbyMailStrings *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4F8;

// ── GetType ──  @ 0x0028211c, 220B
const cType *gcLobbyMailStrings::GetType(void) const {
    if (D_0009F4F8 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4F8 = cType::InitializeType(
            0, 0, 0x1F7, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyMailStrings::New,
            0, 0, 0);
    }
    return D_0009F4F8;
}

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
