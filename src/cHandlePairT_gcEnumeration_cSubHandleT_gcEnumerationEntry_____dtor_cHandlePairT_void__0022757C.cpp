// 0x0022757c cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::~cHandlePairT(void)
// Symbol: __0oMcHandlePairT76NgcEnumeration6LcSubHandleT76SgcEnumerationEntry__dtv
//
// Deleting destructor thunk. SNC emits a (this, flag) signature; when flag&1 the
// object is freed through cMemPool's per-pool deleter, falling back to free().
// The class is trivial (two ints), so there is no member-destruction body.
//
// Template class + explicit instantiation so the emitted text symbol is the
// SNC-mangled __dtv (an in-class inline dtor under explicit instantiation emits
// no text symbol). Mirrors the matched sibling ctors at 0x002275f0 / 0x00234318.
//
// NOTE: a stale, non-matching body for this address exists in
// src/cHandlePairTgcEnumerationcSubHandleTgcEnumerationEntry.cpp using an
// extern "C" name (no DB symbol). It is left untouched per hard-isolation.

class gcEnumeration;
class gcEnumerationEntry;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
}

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class A, class B>
class cHandlePairT {
public:
    int mHandle;
    int mSub;
    ~cHandlePairT();
};

template <class A, class B>
cHandlePairT<A, B>::~cHandlePairT() {
}

template cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::~cHandlePairT();
