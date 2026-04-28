#include "cBase.h"
#include "cFile.h"
#include "cMemPool.h"
#include "cObject.h"

extern char gcTableTemplatevirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const int *);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    cBaseArray &operator=(const cBaseArray &);
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class gcEnumeration {
public:
    int GetSize(void) const;
};

class gcTableTemplate : public cObject {
public:
    gcTableTemplate(cBase *);
    int GetNumRows(void) const;
    void AssignCopy(const cBase *);
    void Reset(cMemPool *, bool);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

gcTableTemplate *dcast(const cBase *);

void gcTableTemplate_gcTableTemplate(gcTableTemplate *, cBase *);
void cObject_Write(const gcTableTemplate *, cFile &);

void cBaseArray_SetSize(void *, int);

extern void *D_00038890[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

int gcTableTemplate::GetNumRows() const {
    int *data = *(int **)((char *)this + 0x50);
    int size = 0;
    if (data != 0) {
        size = data[-1] & 0x3FFFFFFF;
    }
    return size;
}

gcTableTemplate::gcTableTemplate(cBase *parent) : cObject(parent) {
    ((void **)this)[1] = gcTableTemplatevirtualtable;
    ((int *)this)[0x44 / 4] = 0;
    ((int *)this)[0x48 / 4] = 0;
    *(void **)((char *)this + 0x4C) = (void *)this;
    ((int *)this)[0x50 / 4] = 0;
    ((int *)this)[0x54 / 4] = 0;
}

void gcTableTemplate::AssignCopy(const cBase *base) {
    gcTableTemplate *other = dcast(base);
    ((cObject *)this)->operator=(*(cObject *)other);
    *(cHandle *)((char *)this + 0x44) = *(cHandle *)((char *)other + 0x44);
    ((cBaseArray *)((char *)this + 0x48))->operator=(*(cBaseArray *)((char *)other + 0x48));
    ((cArrayBase<int> *)((char *)this + 0x50))->operator=(*(cArrayBase<int> *)((char *)other + 0x50));
    *(int *)((char *)this + 0x54) = *(int *)((char *)other + 0x54);
}

void gcTableTemplate::Reset(cMemPool *pool, bool flag) {
    int handle = *(int *)((char *)this + 0x44);
    cBaseArray *arr = (cBaseArray *)((char *)this + 0x48);
    void *enum_ptr;
    if (handle == 0) {
        enum_ptr = (void *)0;
    } else {
        void *e = D_00038890[handle & 0xFFFF];
        enum_ptr = (void *)0;
        if (e != (void *)0 && *(int *)((char *)e + 0x30) == handle) {
            enum_ptr = e;
        }
    }

    if (enum_ptr != (gcEnumeration *)0) {
        gcEnumeration *e2 = (gcEnumeration *)0;
        if (handle != 0) {
            e2 = (gcEnumeration *)D_00038890[handle & 0xFFFF];
        }
        cBaseArray_SetSize(arr, e2->GetSize());
    } else {
        cBaseArray_SetSize(arr, 0);
    }

    int sz = 0;
    int *data = *(int **)((char *)this + 0x50);
    if (data != (int *)0) {
        sz = data[-1] & 0x3FFFFFFF;
    }
    *(int *)((char *)this + 0x54) = sz;
}

cBase *gcTableTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableTemplate *result = 0;
    gcTableTemplate *obj = (gcTableTemplate *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        gcTableTemplate_gcTableTemplate(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

