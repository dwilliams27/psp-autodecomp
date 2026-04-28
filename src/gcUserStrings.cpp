// gcUserStrings.cpp — gcAll_psp.obj
//
// Functions matched here:
//   0x00291bb0  gcUserStrings::New(cMemPool *, cBase *) static   144B
//   0x00291d1c  gcUserStrings::Write(cFile &) const              100B
//
// Layout: cBase (8) + gcDesiredValue (4) + int (4) = 16 bytes.
// gcStringValue is a base with no extra members and no ctor body.

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcUserStringsclassdesc[];

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

class gcDesiredValue {
public:
    int _data;
    void Write(cWriteBlock &) const;
};

class gcUserStrings : public gcStringValue {
public:
    gcDesiredValue mDesired;   // 0x08
    int mField0C;              // 0x0C

    gcUserStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcUserStringsclassdesc;
        mDesired._data = (int)this | 1;
        mField0C = 0;
    }

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// ── gcUserStrings::Write @ 0x00291d1c ──
void gcUserStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringValue::Write(file);
    mDesired.Write(wb);
    wb.Write(mField0C);
    wb.End();
}

// ── gcUserStrings::New @ 0x00291bb0 ──
cBase *gcUserStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcUserStrings *result = 0;
    gcUserStrings *obj =
        (gcUserStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUserStrings(parent);
        result = obj;
    }
    return (cBase *)result;
}
