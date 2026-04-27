// gcStringTable.cpp — decompiled from gcAll_psp.obj.
// Parent: cObject. Vtable at 0x386E38 (gcStringTableclassdesc).
//
// Functions:
//   0x000d6b98 gcStringTable::Write(cFile &) const            100B
//   0x000d6d00 gcStringTable::gcStringTable(cBase *)           84B
//   0x0023b3e4 gcStringTable::New(cMemPool *, cBase *) static 124B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cBaseArray {
public:
    void *mData;   // +0
    void *mOwner;  // +4
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcStringTable : public cObject {
public:
    char _pad[0x44];     // cObject internals (mField28 halfword lives at 0x28)
    cBaseArray mArray;   // 0x44 (mData) + 0x48 (mOwner)
    bool mFlag;          // 0x4C

    gcStringTable(cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char gcStringTableclassdesc[];   // 0x386E38

extern "C" void gcStringTable_construct(void *self, cBase *parent);

// ── gcStringTable::Write @ 0x000d6b98 ──
void gcStringTable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cObject::Write(file);
    mArray.Write(wb);
    wb.Write(mFlag);
    wb.End();
}

// ── gcStringTable::New(cMemPool *, cBase *) static @ 0x0023b3e4 ──
cBase *gcStringTable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStringTable *result = 0;
    gcStringTable *obj = (gcStringTable *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        gcStringTable_construct(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStringTable::gcStringTable(cBase *) @ 0x000d6d00 ──
gcStringTable::gcStringTable(cBase *parent) : cObject(parent) {
    ((void **)this)[1] = gcStringTableclassdesc;
    mArray.mData = 0;
    mArray.mOwner = this;
    mFlag = true;
    unsigned short tmp = (unsigned short)(*(unsigned short *)((char *)this + 0x28) | 0x80);
    *(unsigned short *)((char *)this + 0x28) = tmp;
}
