// gcEntityControllerTemplate
// Decompiled functions:
//   0x0010fed8 gcEntityControllerTemplate::Reset(cMemPool *, bool)
//   0x0025cbac gcEntityControllerTemplate::GetType(void) const
//   0x00110030 gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const

class cBase;
class cMemPool;
class gcEnumeration;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void Reset(cMemPool *);
};

template <class T>
struct cHandleT {
    int mIndex;
    bool operator==(const cHandleT &other) const { return mIndex == other.mIndex; }
};

class gcEntityControllerTemplate {
public:
    void *mClassDesc;       // 0x00
    int _pad04;             // 0x04
    cBaseArray mArr1;       // 0x08
    int _pad10[3];          // 0x10..0x1B
    cBaseArray mArr2;       // 0x1C

    void Reset(cMemPool *, bool);
    const cType *GetType(void) const;
    int FindAnimationSet(cHandleT<gcEnumeration>) const;
};

// Set entry — cHandleT<gcEnumeration> at +8 (same shape as gcPartialBodyControllerSet)
class gcEntityControllerSet {
public:
    char _pad[8];
    cHandleT<gcEnumeration> mEnumeration;   // +8
};

extern cType *D_000385DC;
extern cType *D_0009A400;

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
        if (*eh == h) {
            return i;
        }
        i++;
        offset += 4;
    }
}
