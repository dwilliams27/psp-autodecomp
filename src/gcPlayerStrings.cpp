// gcPlayerStrings.cpp — decompiled from gcAll_psp.obj.
// Parent: gcStringLValue.
//
// Functions:
//   0x0031e100 gcPlayerStrings::New(cMemPool *, cBase *) static  144B
//   0x0031e2a8 gcPlayerStrings::Write(cFile &) const             100B

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

extern char gcStringLValueclassdesc[];
extern char gcPlayerStringsclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcStringLValue {
public:
    cBase *mOwner;       // 0
    void *mClassDesc;    // 4
    gcStringLValue(cBase *owner) {
        mClassDesc = gcStringLValueclassdesc;
        mOwner = owner;
    }
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mOwner;          // 0 (within gcDesiredValue)
    void Write(cWriteBlock &) const;
};

class gcPlayerStrings : public gcStringLValue {
public:
    gcDesiredValue mDesired;   // offset 8
    unsigned int mValue;       // offset 12

    gcPlayerStrings(cBase *parent) : gcStringLValue(parent) {
        mClassDesc = gcPlayerStringsclassdesc;
        *(int *)((char *)this + 8) = (int)this | 1;
        *(int *)((char *)this + 12) = 0;
    }

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// ── Write @ 0x0031e2a8 ──
void gcPlayerStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringLValue::Write(file);
    mDesired.Write(wb);
    wb.Write(mValue);
    wb.End();
}

// ── New @ 0x0031e100 ──
cBase *gcPlayerStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcPlayerStrings *result = 0;
    gcPlayerStrings *obj = (gcPlayerStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcPlayerStrings(parent);
        result = obj;
    }
    return (cBase *)result;
}
