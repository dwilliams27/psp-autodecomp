// gcEnumeration.cpp — decompiled from gcAll_psp.obj.
// Parent: cObject. Same shape as gcStringTable / gcConfiguration but with
// extra payload — cBaseArray at 0x44, u8 at 0x4C, int at 0x50, cHandle at 0x54.
//
// Functions:
//   0x000d3eb4 gcEnumeration::Write(cFile &) const            124B
//   0x00238d04 gcEnumeration::AssignCopy(const cBase *)       108B

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

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
    void Write(unsigned char);
    void Write(int);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct gcEnumerationGetResult {
    int owner;
    int value;
};

struct HandleEntry {
    char pad[0x30];
    int handle;
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000998F0;
extern HandleEntry *D_00048890[];

class gcEnumeration : public cObject {
public:
    char _pad[0x44];        // cObject internals
    cBaseArray mArray;      // 0x44 (8 bytes)
    unsigned char mField4C; // 0x4C
    char _pad2[3];
    int mField50;           // 0x50
    cHandle mHandle;        // 0x54

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    gcEnumerationGetResult Get(int) const;
    int IsIndexed(void) const;
    ~gcEnumeration();
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// ── gcEnumeration::AssignCopy(const cBase *) @ 0x00238d04 ──
void gcEnumeration::AssignCopy(const cBase *src) {
    gcEnumeration *other = dcast<gcEnumeration>(src);
    cObject::operator=(*other);
    mArray = other->mArray;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mHandle = other->mHandle;
}

// ── gcEnumeration::GetType(void) const @ 0x00238e0c ──
const cType *gcEnumeration::GetType(void) const {
    if (D_000998F0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000998F0 = cType::InitializeType(0, 0, 0xAB, D_000385E4,
                                           &gcEnumeration::New,
                                           (const char *)0x36D8A4,
                                           (const char *)0x36D8B4,
                                           5);
    }
    return D_000998F0;
}

// ── gcEnumeration::Write(cFile &) const @ 0x000d3eb4 ──
void gcEnumeration::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cObject::Write(file);
    mArray.Write(wb);
    wb.Write(mField4C);
    wb.Write(mField50);
    mHandle.Write(wb);
    wb.End();
}

// ── gcEnumeration::Get(int) const @ 0x000d478c ──
gcEnumerationGetResult gcEnumeration::Get(int index) const {
    int handle = *(int *)((const char *)this + 0x54);
    HandleEntry *entry;
    if (handle != 0) {
        HandleEntry *candidate = D_00048890[handle & 0xFFFF];
        entry = 0;
        if (candidate != 0) {
            if (candidate->handle == handle) {
                entry = candidate;
            }
        }
    } else {
        entry = 0;
    }

    if (entry != 0) {
        gcEnumeration *target = 0;
        if (handle != 0) {
            target = (gcEnumeration *)D_00048890[handle & 0xFFFF];
        }
        gcEnumerationGetResult child = target->Get(index);
        int value = child.value;
        gcEnumerationGetResult result;
        result.owner = 0;
        if (this != 0) {
            result.owner = *(int *)((const char *)this + 0x30);
        }
        result.value = value;
        return result;
    }

    if (IsIndexed()) {
        int value = (index & 0xFFFF) | 0x10000;
        gcEnumerationGetResult result;
        result.owner = 0;
        if (this != 0) {
            result.owner = *(int *)((const char *)this + 0x30);
        }
        result.value = value;
        return result;
    }

    if (index >= 0) {
        void **items = (void **)*(void **)((const char *)this + 0x44);
        int count = 0;
        if (items != 0) {
            count = ((int *)items)[-1];
        }
        if (index < count) {
            void *item = items[index];
            if (item != 0) {
                int value = *(int *)((char *)item + 0x24);
                gcEnumerationGetResult result;
                result.owner = 0;
                if (this != 0) {
                    result.owner = *(int *)((const char *)this + 0x30);
                }
                result.value = value;
                return result;
            }
        }
    }

    int value = 0;
    gcEnumerationGetResult result;
    result.owner = 0;
    if (this != 0) {
        result.owner = *(int *)((const char *)this + 0x30);
    }
    result.value = value;
    return result;
}

// Original object keeps this branch-loop pad inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oNgcEnumerationdtv, 0x98\n");

// ── gcEnumeration::~gcEnumeration(void) @ 0x00239628 ──
gcEnumeration::~gcEnumeration() {
    *(void **)((char *)this + 4) = (void *)0x386BB8;
    if ((char *)this + 0x44 != 0) {
        ((cBaseArray *)((char *)this + 0x44))->RemoveAll();
    }
}
