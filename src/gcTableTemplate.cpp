#include "cBase.h"
#include "cFile.h"
#include "cMemPool.h"
#include "cObject.h"
#include "cOutStream.h"

extern char gcTableTemplatevirtualtable[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

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
    const cType *GetType(void) const;
    int GetNumRows(void) const;
    void AssignCopy(const cBase *);
    void Reset(cMemPool *, bool);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void Write(cOutStream &) const;
};

gcTableTemplate *dcast(const cBase *);

void gcTableTemplate_gcTableTemplate(gcTableTemplate *, cBase *);
void cObject_Write(const gcTableTemplate *, cFile &);

void cBaseArray_SetSize(void *, int);

extern void *D_00038890[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F494;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, cOutStream &);
};

struct TypedObject {
    char pad0[4];
    char *mType;
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

const cType *gcTableTemplate::GetType(void) const {
    if (D_0009F494 == 0) {
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
        D_0009F494 = cType::InitializeType(0, 0, 0x216, D_000385E4,
                                           &gcTableTemplate::New,
                                           (const char *)0x36D9CC,
                                           (const char *)0x36D9DC, 5);
    }
    return D_0009F494;
}

void gcTableTemplate::Write(cOutStream &stream) const {
    void **temp_a1 = *(void ***)((char *)this + 0x48);
    int var_a0 = 0;
    if (temp_a1 != 0) {
        var_a0 = ((int *)temp_a1)[-1];
    }
    stream.Write(var_a0, 0x20, true);

    int var_s3 = 0;
    int var_s4 = 0;
loop_3:
    int var_a1 = 0;
    void **temp_a0_2 = *(void ***)((char *)this + 0x48);
    if (temp_a0_2 != 0) {
        var_a1 = ((int *)temp_a0_2)[-1];
    }
    if (var_s3 < var_a1) {
        TypedObject *temp_a0 = *(TypedObject **)((char *)temp_a0_2 + var_s4);
        int temp_a3 = stream.mBitPos;
        int temp_a1_2 = temp_a3 & 7;
        unsigned char *temp_a2 = stream.mData + (temp_a3 >> 3);
        int temp_a3_2 = temp_a3 + 1;
        if (temp_a0 != 0) {
            stream.mBitPos = temp_a3_2;
            unsigned char temp_a3_3 = stream.mOverflow;
            int var_a3;
            if (temp_a3_3 == 0) {
                var_a3 = temp_a3_3 == 0;
                if (stream.mCapacity < ((int)(stream.mBitPos + 7) >> 3)) {
                    stream.mOverflow = 1U;
                    goto block_10;
                }
            } else {
block_10:
                var_a3 = stream.mOverflow == 0;
            }
            if (var_a3 & 0xFF) {
                *temp_a2 = (unsigned char)((*temp_a2 & ~(1 << temp_a1_2)) |
                                           (1 << temp_a1_2));
            }
            char *temp_a2_2 = temp_a0->mType;
            short temp_a3_4 = ((TypeDispatchEntry *)(temp_a2_2 + 0x70))->offset;
            void (*temp_a2_3)(void *, cOutStream &) =
                ((TypeDispatchEntry *)(temp_a2_2 + 0x70))->fn;
            temp_a2_3((char *)temp_a0 + temp_a3_4, stream);
        } else {
            stream.mBitPos = temp_a3_2;
            unsigned char temp_a3_5 = stream.mOverflow;
            int var_a0_2 = temp_a3_5 == 0;
            if (temp_a3_5 == 0) {
                var_a0_2 = temp_a3_5 == 0;
                if (stream.mCapacity < ((int)(stream.mBitPos + 7) >> 3)) {
                    stream.mOverflow = 1U;
                    var_a0_2 = stream.mOverflow == 0;
                }
            }
            if (var_a0_2 & 0xFF) {
                *temp_a2 = (unsigned char)((*temp_a2 & ~(1 << temp_a1_2)) |
                                           (0 << temp_a1_2));
            }
        }
        var_s3 += 1;
        var_s4 += 4;
        goto loop_3;
    }
}
