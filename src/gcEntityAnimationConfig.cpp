// Decompiled functions from gcAll_psp.obj
//   0x000e9854 gcEntityAnimationConfig::gcEntityAnimationConfig(cBase *)
//   0x00240168 gcEntityAnimationConfig::New(cMemPool *, cBase *) static
//   0x002401e4 gcEntityAnimationConfig::GetType(void) const
//   0x00240284 gcEntityAnimationConfig::GetName(char *) const
//   0x0024003c gcEntityAnimationConfig::AssignCopy(const cBase *)

#include "cBase.h"

class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    int handle = mUnk08;
    NameLookupEntry *entry = 0;
    if (handle != 0) {
        NameLookupEntry *e = gcAnimNameTable[handle & 0xFFFF];
        if (e != 0 && e->mKey == handle) {
            entry = e;
        }
    }
    const char *str;
    if (entry != 0) {
        str = (const char *)entry + 8;
        if (entry->mCount == 0) {
            str = gcAnimName_unnamed;
        }
    } else if (handle != 0) {
        str = gcAnimName_invalid;
    } else {
        str = gcAnimName_none;
    }
    cStrCopy(dest, str);
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
        const cType *myType = type_gcEntityAnimationConfig;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *otherType = getType((char *)other + voff);

        int ok = 0;
        if (myType != 0 && otherType != 0) {
            cTypeNode *t = (cTypeNode *)otherType;
            do {
                if (t == (cTypeNode *)myType) {
                    ok = 1;
                    break;
                }
                t = t->parent;
            } while (t != 0);
        }
        if (ok) {
            var_s2 = other;
        }
    }
    *this = *(const gcEntityAnimationConfig *)var_s2;
}
