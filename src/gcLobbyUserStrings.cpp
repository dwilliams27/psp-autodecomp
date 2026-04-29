// gcLobbyUserStrings — string-table value bound to a desired-value lvalue.
//
// Functions matched here:
//   gcLobbyUserStrings::Write(cFile &) const            @ 0x00284888  (gcAll_psp.obj)
//   gcLobbyUserStrings::New(cMemPool *, cBase *) static @ 0x0028471c  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cBase;

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
    const cType *GetType(void) const;
};

class gcNetworkConfigStrings {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcPartialBodyControllerTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F504;
extern cType *D_0009F554;
extern cType *D_0009F5DC;
extern cType *D_0009F5F0;

// ── GetType ──  @ 0x002847ac, 220B
const cType *gcLobbyUserStrings::GetType(void) const {
    if (D_0009F504 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F504 = cType::InitializeType(
            0, 0, 0x18C, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyUserStrings::New,
            0, 0, 0);
    }
    return D_0009F504;
}

// ── GetType ──  @ 0x00285e78, 220B
const cType *gcNetworkConfigStrings::GetType(void) const {
    if (D_0009F554 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F554 = cType::InitializeType(0, 0, 0xF8, D_0009F454,
                                           &gcNetworkConfigStrings::New,
                                           0, 0, 0);
    }
    return D_0009F554;
}

// ── GetType ──  @ 0x002a55fc, 220B
const cType *gcPartialBodyControllerTemplate::GetType(void) const {
    if (D_0009F5F0 == 0) {
        if (D_0009F5DC == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5DC = cType::InitializeType(0, 0, 0x104, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F5F0 = cType::InitializeType(0, 0, 0x106, D_0009F5DC,
                                           &gcPartialBodyControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F5F0;
}

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
