// Decompiled functions from gcAll_psp.obj
//   0x000e9854 gcEntityAnimationConfig::gcEntityAnimationConfig(cBase *)
//   0x00240168 gcEntityAnimationConfig::New(cMemPool *, cBase *) static
//   0x002401e4 gcEntityAnimationConfig::GetType(void) const
//   0x00240284 gcEntityAnimationConfig::GetName(char *) const
//   0x0024003c gcEntityAnimationConfig::AssignCopy(const cBase *)

#include "cBase.h"

class cMemPool;
class cFile;
class cFileHandle;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;
    int mPos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &, cMemPool *);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

template <class T> class cHandleT {
};

class eSoundData;

template <class T> class cArray {
public:
    void Read(cReadBlock &);
};

class gcEntityAnimationConfig {
public:
    cBase *mOwner;       // 0x00
    void *mVTable;       // 0x04
    int mUnk08;          // 0x08
    int mUnk0C;          // 0x0C
    int mUnk10;          // 0x10 = this|1
    int mUnk14;          // 0x14 = this|1
    int mUnk18;          // 0x18
    int mUnk1C;          // 0x1C
    void *mUnk20;        // 0x20 = this

    gcEntityAnimationConfig(cBase *);
    void GetName(char *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    gcEntityAnimationConfig &operator=(const gcEntityAnimationConfig &);
};

extern char gcEntityAnimationConfigvirtualtable[];
extern const char gcEntityAnimationConfig_base_name[];
extern const char gcEntityAnimationConfig_base_desc[];

static cType *type_base;
static cType *type_gcEntityAnimationConfig;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct NameLookupEntry {
    char name[8];        // 0x00..0x08 (name starts here via +8 access)
    int pad08[5];        // fill up to 0x1C
    short mCount;        // 0x1C
    char pad1E[0x12];
    int mKey;            // 0x30
};

void gcEntityAnimationConfig_ctor(gcEntityAnimationConfig *, cBase *);
void cStrCopy(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// ============================================================
// 0x000e9854 — constructor
// ============================================================
gcEntityAnimationConfig::gcEntityAnimationConfig(cBase *owner) {
    mOwner = owner;
    mVTable = gcEntityAnimationConfigvirtualtable;
    mUnk08 = 0;
    mUnk0C = 0;
    int enc = (int)this | 1;
    mUnk10 = enc;
    mUnk14 = enc;
    mUnk18 = 0;
    mUnk1C = 0;
    mUnk20 = this;
}

// ============================================================
// 0x00240168 — New(cMemPool *, cBase *)
// ============================================================
cBase *gcEntityAnimationConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAnimationConfig *result = 0;
    gcEntityAnimationConfig *obj = (gcEntityAnimationConfig *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcEntityAnimationConfig_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x002401e4 — GetType
// ============================================================
const cType *gcEntityAnimationConfig::GetType(void) const {
    if (!type_gcEntityAnimationConfig) {
        if (!type_base) {
            type_base = cType::InitializeType(gcEntityAnimationConfig_base_name,
                                              gcEntityAnimationConfig_base_desc,
                                              1, 0, 0, 0, 0, 0);
        }
        type_gcEntityAnimationConfig = cType::InitializeType(0, 0, 0xB0, type_base,
                                                             gcEntityAnimationConfig::New,
                                                             0, 0, 0);
    }
    return type_gcEntityAnimationConfig;
}

// ============================================================
// 0x00240284 — GetName
// ============================================================
extern NameLookupEntry *gcAnimNameTable[];
extern const char gcAnimName_unnamed[];   // 0x36DAB8
extern const char gcAnimName_invalid[];   // 0x36DAC4
extern const char gcAnimName_none[];      // 0x36DACC

void gcEntityAnimationConfig::GetName(char *dest) const {
    char *out = dest;
    int handle = *(const int *)((const char *)this + 8);
    NameLookupEntry *entry;
    if (handle == 0) {
        entry = 0;
    } else {
        entry = 0;
        NameLookupEntry *cand = gcAnimNameTable[handle & 0xFFFF];
        if (cand != 0 && cand->mKey == handle) {
            entry = cand;
        }
    }
    const char *str;
    if (entry != 0) {
        str = (const char *)entry + 8;
        unsigned char is_empty = (entry->mCount == 0) & 0xFF;
        if (is_empty != 0) {
            str = gcAnimName_unnamed;
        }
        __asm__ volatile("" ::: "memory");
    } else if (handle != 0) {
        str = gcAnimName_invalid;
    } else {
        str = gcAnimName_none;
    }
    cStrCopy(out, str);
}

// ============================================================
// 0x000e96e4 — Read(cFile &, cMemPool *)
// ============================================================
int gcEntityAnimationConfig::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$17") = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);

    if (rb[3] != 5) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    *(int *)((char *)this + 0x08) = 0;
    cHandle *handle = (cHandle *)((char *)this + 0x08);
    handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));
    ((cArray<cHandleT<eSoundData> > *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x18, 4);

    int sp14;
    int value = *(int *)((char *)this + 0x14);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x14);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x14) = newValue;
    ((cBaseArray *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ============================================================
// 0x0024003c — AssignCopy
// ============================================================
struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

void gcEntityAnimationConfig::AssignCopy(const cBase *other) {
    const cBase *var_s2 = 0;
    if (other != 0) {
        if (!type_gcEntityAnimationConfig) {
            if (!type_base) {
                type_base = cType::InitializeType(gcEntityAnimationConfig_base_name,
                                                  gcEntityAnimationConfig_base_desc,
                                                  1, 0, 0, 0, 0, 0);
            }
            type_gcEntityAnimationConfig = cType::InitializeType(0, 0, 0xB0, type_base,
                                                                 gcEntityAnimationConfig::New,
                                                                 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        void *myType = type_gcEntityAnimationConfig;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        void *(*getType)(void *) = (void *(*)(void *))slot->getType;
        void *otherType = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done_type;
        }
        if (otherType != 0) {
        loop_type:
            if (otherType == myType) {
                ok = 1;
                goto done_type;
            }
            otherType = *(void **)((char *)otherType + 0x1C);
            if (otherType != 0) {
                goto loop_type;
            }
        }
        ok = 0;
    done_type:
        if (ok) {
            var_s2 = other;
        }
    }
    *this = *(const gcEntityAnimationConfig *)var_s2;
}

// ============================================================
// 0x000e9508 — Write(cFile &) const
// ============================================================
void gcEntityAnimationConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const cHandle *)((const char *)this + 8))->Write(wb);

    int *temp_a0 = *(int *const *)((const char *)this + 0xC);
    int var_s1 = 0;
    if (temp_a0 != 0) {
        var_s1 = temp_a0[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_s1);

    int *temp_a0_2 = *(int *const *)((const char *)this + 0xC);
    int var_s1_2 = 0;
    if (temp_a0_2 != 0) {
        var_s1_2 = temp_a0_2[-1] & 0x3FFFFFFF;
    }
    int var_s2 = 0;
    if (var_s2 < var_s1_2) {
        int var_s3 = 0;
        int *var_a0_2 = temp_a0_2;
        var_s3 = (int)var_a0_2 + var_s3;
loop_1:
        {
            ((const cHandle *)var_s3)->Write(wb);
            var_s2 += 1;
            if (var_s2 < var_s1_2) {
                var_s3 += 4;
                goto loop_1;
            }
        }
    }

    wb.Write(*(const unsigned int *)((const char *)this + 0x18));

    int var_a0 = *(const int *)((const char *)this + 0x14);
    int var_a2 = 0;
    int temp_a1 = var_a0 & 1;
    if (temp_a1 != 0) {
        var_a2 = 1;
    }

    if (var_a2 == 0) {
        var_a2 = ((var_a0 == 0) & 0xFF) != 0;
    } else {
        var_a2 = 1;
    }

    int var_a2_3 = 0;
    if (var_a2 == 0) {
        if (temp_a1 != 0) {
            var_a2_3 = 1;
        }

        int var_a2_4 = var_a0;
        if (var_a2_3 != 0) {
            var_a2_4 = 0;
        }

        int var_a3 = 0;
        int var_a2_5;
        if (((*(int *)((char *)var_a2_4 + 0x10) == 0) & 0xFF)
            && ((*(int *)((char *)var_a2_4 + 0x2C) == 0) & 0xFF)
            && ((*(int *)((char *)var_a2_4 + 0x48) == 0) & 0xFF)) {
            var_a2_5 = 0 & 0xFF;
            if ((*(int *)((char *)var_a2_4 + 0x64) == 0) & 0xFF) {
                var_a3 = 1;
                goto block_22;
            }
        } else {
block_22:
            var_a2_5 = var_a3 & 0xFF;
        }

        var_a2_3 = temp_a1;
        if (var_a2_5 != 0) {
            wb.WriteBase(0);
        } else {
            int var_a1 = 0;
            if (var_a2_3 != 0) {
                var_a1 = 1;
            }
            if (var_a1 != 0) {
                var_a0 = 0;
            }
            wb.WriteBase((const cBase *)var_a0);
        }
    } else {
        wb.WriteBase(0);
    }

    ((const cBaseArray *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    wb.End();
}
