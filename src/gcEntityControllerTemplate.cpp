// gcEntityControllerTemplate
// Decompiled functions:
//   0x0010fc58 gcEntityControllerTemplate::Write(cFile &) const
//   0x0010fe40 gcEntityControllerTemplate::gcEntityControllerTemplate(cBase *)
//   0x0010fed8 gcEntityControllerTemplate::Reset(cMemPool *, bool)
//   0x00110030 gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const
//   0x001101a8 gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const
//   0x0025cbac gcEntityControllerTemplate::GetType(void) const
//   0x0025d4c8 gcEntityControllerTemplate::~gcEntityControllerTemplate(void)

class cBase;
class cFile;
class cMemPool;
class gcEnumeration;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void Write(float);
    void End(void);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void Reset(cMemPool *);
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

template <class T>
struct cHandleT {
    int mIndex;
    bool operator==(const cHandleT &other) const { return mIndex == other.mIndex; }
    void Write(cWriteBlock &) const;
};

class gcEntityControllerTemplate {
public:
    cBase *mParent;                    // 0x00
    void *mVtable;                     // 0x04
    cBaseArray mArr1;                  // 0x08
    cHandleT<gcEnumeration> mHandles[2]; // 0x10..0x17
    int _pad18;                        // 0x18
    cBaseArray mArr2;                  // 0x1C..0x23
    int _pad24[3];                     // 0x24..0x2F
    float mFloats3[4];                 // 0x30..0x3F
    float mFloats2[2];                 // 0x40..0x47
    float mFloat48;                    // 0x48
    int _pad4C;                        // 0x4C

    ~gcEntityControllerTemplate();
    void Reset(cMemPool *, bool);
    const cType *GetType(void) const;
    void GetName(char *) const;
    void Write(cFile &) const;
    int FindAttackSet(cHandleT<gcEnumeration>) const;
    int FindAnimationSet(cHandleT<gcEnumeration>) const;
    static void operator delete(void *p);
};

class cXML {
public:
    class cNode {
    public:
        int SetText(const char *);
    };

    char _pad0[4];
    unsigned char mFlag4;

    cNode *Find(const char *, bool);
    bool SetText(const char *, const char *, bool);
};

// Set entry — cHandleT<gcEnumeration> at +8 (same shape as gcPartialBodyControllerSet)
class gcEntityControllerSet {
public:
    char _pad[8];
    cHandleT<gcEnumeration> mEnumeration;   // +8
};

extern cType *D_000385DC;
extern cType *D_0009A400;
extern char gcEntityControllerTemplate_dtor_classdesc[];
extern char cBase_dtor_classdesc[];
extern "C" void free(void *);
void cStrCopy(char *, const char *);

struct gcNameHandleEntry {
    char _pad0[0x1C];
    short mNameIndex;
    char _pad1E[0x12];
    int mHandle;
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

inline void gcEntityControllerTemplate::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oagcEntityControllerTemplatedtv, 0xcc\n");

// =====================================================================
// 0x0025d4c8 — ~gcEntityControllerTemplate(void)
// =====================================================================
gcEntityControllerTemplate::~gcEntityControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    cBaseArray *arr1 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *arr0 = (cBaseArray *)((char *)this + 0x08);
    if (arr1 != 0) {
        arr1->RemoveAll();
    }
    if (arr0 != 0) {
        arr0->RemoveAll();
    }
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}

// =====================================================================
// 0x0010fed8 — Reset(cMemPool *, bool)
// =====================================================================
void gcEntityControllerTemplate::Reset(cMemPool *pool, bool /*flag*/) {
    mArr1.Reset(pool);
    mArr2.Reset(pool);
}

// =====================================================================
// 0x0025cbac — GetType(void) const
// =====================================================================
const cType *gcEntityControllerTemplate::GetType(void) const {
    if (D_0009A400 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009A400;
}

// =====================================================================
// 0x0025cc44 — GetName(char *) const
// =====================================================================
void gcEntityControllerTemplate::GetName(char *buf) const {
    const char *base = (const char *)this + 0x10;
    volatile int *slot1 = (volatile int *)base;
    volatile int *slot2 = (volatile int *)(base + 4);
    int handle2 = *slot2;
    gcNameHandleEntry *valid2;
    if (handle2 == 0) {
        valid2 = 0;
    } else {
        char *table = (char *)0x38890;
        gcNameHandleEntry *candidate =
            *(gcNameHandleEntry **)(table + ((handle2 & 0xFFFF) << 2));
        valid2 = 0;
        if (candidate != 0 && candidate->mHandle == handle2) {
            valid2 = candidate;
        }
    }

    void *name;
    if (valid2 != 0) {
        int handle = *slot2;
        gcNameHandleEntry *entry;
        if (handle == 0) {
            entry = 0;
        } else {
            char *table = (char *)0x38890;
            gcNameHandleEntry *candidate =
                *(gcNameHandleEntry **)(table + ((handle & 0xFFFF) << 2));
            entry = 0;
            if (candidate != 0 && candidate->mHandle == handle) {
                entry = candidate;
            }
        }
        if (entry != 0) {
            int unnamed = ((unsigned int)(int)entry->mNameIndex < 1u);
            unnamed &= 0xFF;
            name = (char *)entry + 8;
            if (unnamed != 0) {
                name = (void *)0x36DAB8;
            }
            __asm__ volatile("" : : "r"(name));
            goto copy;
        } else if (handle != 0) {
            name = (void *)0x36DAC4;
        } else {
            name = (void *)0x36DACC;
        }
    } else {
        int handle = *slot1;
        gcNameHandleEntry *entry;
        if (handle == 0) {
            entry = 0;
        } else {
            char *table = (char *)0x38890;
            gcNameHandleEntry *candidate =
                *(gcNameHandleEntry **)(table + ((handle & 0xFFFF) << 2));
            entry = 0;
            if (candidate != 0 && candidate->mHandle == handle) {
                entry = candidate;
            }
        }
        if (entry != 0) {
            int unnamed = ((unsigned int)(int)entry->mNameIndex < 1u);
            unnamed &= 0xFF;
            name = (char *)entry + 8;
            if (unnamed != 0) {
                name = (void *)0x36DAB8;
            }
            __asm__ volatile("" : : "r"(name));
            goto copy;
        } else if (handle != 0) {
            name = (void *)0x36DAC4;
        } else {
            name = (void *)0x36DACC;
        }
    }
copy:
    cStrCopy(buf, (const char *)name);
}

// =====================================================================
// 0x0000d470 — cXML::SetText(const char *, const char *, bool)
// =====================================================================
bool cXML::SetText(const char *name, const char *text, bool createIfMissing) {
    cNode *node = Find(name, createIfMissing);
    if (node != 0) {
        if (node->SetText(text) != 0) {
            return true;
        }
        mFlag4 = 0;
    }
    return false;
}

// =====================================================================
// 0x00110030 — FindAnimationSet(cHandleT<gcEnumeration>) const
// =====================================================================
int gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration> h) const {
    int i = 0;
    void *data = mArr1.mData;
    int offset = 0;
    while (1) {
        int size = 0;
        if (data != 0) {
            size = ((int *)data)[-1];
        }
        if (i >= size) {
            return -1;
        }
        gcEntityControllerSet *e =
            *(gcEntityControllerSet **)((char *)data + offset);
        const cHandleT<gcEnumeration> *eh =
            (const cHandleT<gcEnumeration> *)((const char *)e + 8);
        if (((eh->mIndex ^ h.mIndex) == 0) & 0xFF) {
            return i;
        }
        i++;
        offset += 4;
    }
}

// =====================================================================
// 0x001101a8 — FindAttackSet(cHandleT<gcEnumeration>) const
// =====================================================================
int gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration> h) const {
    int i = 0;
    void *data = mArr2.mData;
    int offset = 0;
    while (1) {
        int size = 0;
        if (data != 0) {
            size = ((int *)data)[-1];
        }
        if (i >= size) {
            return -1;
        }
        gcEntityControllerSet *e =
            *(gcEntityControllerSet **)((char *)data + offset);
        const cHandleT<gcEnumeration> *eh =
            (const cHandleT<gcEnumeration> *)((const char *)e + 8);
        if (((eh->mIndex ^ h.mIndex) == 0) & 0xFF) {
            return i;
        }
        i++;
        offset += 4;
    }
}

// =====================================================================
// 0x0010fc58 — Write(cFile &) const
// =====================================================================
void gcEntityControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    mArr1.Write(wb);
    const char *base = (const char *)mHandles;
    int offset = 0;
    int i = 0;
    const cHandleT<gcEnumeration> *p = (const cHandleT<gcEnumeration> *)(base + offset);
    do {
        p->Write(wb);
        i++;
        p++;
    } while (i < 2);
    mArr2.Write(wb);
    wb.Write(3, mFloats3);
    wb.Write(2, mFloats2);
    wb.Write(mFloat48);
    wb.End();
}

// 0x0010fe40 — gcEntityControllerTemplate(cBase *) — NOT YET MATCHED, see notes
