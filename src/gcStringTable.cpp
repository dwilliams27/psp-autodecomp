// gcStringTable.cpp — decompiled from gcAll_psp.obj.
// Parent: cObject. Vtable at 0x386E38 (gcStringTableclassdesc).
//
// Functions:
//   0x000d6b98 gcStringTable::Write(cFile &) const            100B
//   0x000d6d00 gcStringTable::gcStringTable(cBase *)           84B
//   0x0023b3e4 gcStringTable::New(cMemPool *, cBase *) static 124B

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class gcString;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

template <class T> T *dcast(const cBase *);

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
    cBaseArray &operator=(const cBaseArray &);
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcStringTable : public cObject {
public:
    char _pad[0x44];     // cObject internals (mField28 halfword lives at 0x28)
    cBaseArray mArray;   // 0x44 (mData) + 0x48 (mOwner)
    bool mFlag;          // 0x4C

    gcStringTable(cBase *);
    ~gcStringTable();
    void AssignCopy(const cBase *);
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > GetStringHandle(int) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
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

// ── gcStringTable::~gcStringTable(void) @ 0x000d6d54 ──
gcStringTable::~gcStringTable() {
    ((void **)this)[1] = gcStringTableclassdesc;
    void *arr = (char *)this + 0x44;
    if (arr != 0) {
        ((cBaseArray *)arr)->RemoveAll();
    }
}

// ── gcStringTable::GetStringHandle(int) const @ 0x000d6f48 ──
cHandlePairT<gcStringTable, cSubHandleT<gcString> >
gcStringTable::GetStringHandle(int index) const {
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > result;
    cSubHandleT<gcString> sub;
    cSubHandleT<gcString> empty;
    if (index >= 0) {
        void **data = (void **)mArray.mData;
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1];
        }
        if (index < count) {
            void *entry = data[index];
            if (entry != 0) {
                entry = (char *)entry + 0x20;
                sub.mIndex = *(int *)entry;
                cSubHandleT<gcString> *subp = &sub;
                int handle = 0;
                if (this != 0) {
                    handle = *(int *)((char *)this + 0x30);
                }
                result.mHandle.mIndex = handle;
                result.mSubHandle.mIndex = subp->mIndex;
                return result;
            }
        }
    }

    empty.mIndex = 0;
    cSubHandleT<gcString> *subp = &empty;
    int handle = 0;
    if (this != 0) {
        handle = *(int *)((char *)this + 0x30);
    }
    result.mHandle.mIndex = handle;
    result.mSubHandle.mIndex = subp->mIndex;
    return result;
}

// ── gcStringTable::AssignCopy(const cBase *) @ 0x0023b390 ──
void gcStringTable::AssignCopy(const cBase *base) {
    gcStringTable *other = dcast<gcStringTable>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cBaseArray *)((char *)this + 0x44))->operator=(
        *(const cBaseArray *)((char *)other + 0x44));
    mFlag = other->mFlag;
}
