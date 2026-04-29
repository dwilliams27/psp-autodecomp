// gcLobbyGameStrings — string-lvalue with constant flag and desired-value slot.
//
// Functions matched here:
//   gcLobbyGameStrings::Write(cFile &) const            @ 0x002815b0  (gcAll_psp.obj)
//   gcLobbyGameStrings::New(cMemPool *, cBase *) static @ 0x00281400  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cBase;
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char cBaseclassdesc[];
extern char gcLobbyGameStringsclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F4F4;

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

class gcStringLValue : public gcStringValue {
public:
    gcStringLValue(cBase *parent) : gcStringValue(parent) {}
    void Write(cFile &) const;
};

class gcLobbyGameStrings : public gcStringLValue {
public:
    int mField08;       // 0x08 — constant 1
    int mField0C;       // 0x0C — gcDesiredValue first slot, holds (this | 1)
    int mField10;       // 0x10

    gcLobbyGameStrings(cBase *parent) : gcStringLValue(parent) {
        mClassDesc = gcLobbyGameStringsclassdesc;
        mField08 = 1;
        mField0C = (int)this | 1;
        mField10 = 0;
    }
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static gcLobbyGameStrings *New(cMemPool *, cBase *);
};

// ── Write ──  @ 0x002815b0, 112B
void gcLobbyGameStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcStringLValue::Write(file);
    wb.Write(mField08);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mField10);
    wb.End();
}

const cType *gcLobbyGameStrings::GetType(void) const {
    if (D_0009F4F4 == 0) {
        if (D_0009F458 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                               0, 0, 0, 0);
        }
        D_0009F4F4 = cType::InitializeType(
            0, 0, 0x184, D_0009F458,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyGameStrings::New,
            0, 0, 0);
    }
    return D_0009F4F4;
}

// ── New ──  @ 0x00281400, 152B
gcLobbyGameStrings *gcLobbyGameStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyGameStrings *result = 0;
    gcLobbyGameStrings *obj = (gcLobbyGameStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyGameStrings(parent);
        result = obj;
    }
    return result;
}
