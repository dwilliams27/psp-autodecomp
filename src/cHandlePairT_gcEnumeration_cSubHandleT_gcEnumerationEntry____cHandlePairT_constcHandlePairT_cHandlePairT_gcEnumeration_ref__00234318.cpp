// 0x00234318 cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::cHandlePairT(const cHandlePairT &)
// Copy constructor: copy two sentinel ints from source, return this.
// Symbol: __0oMcHandlePairT76NgcEnumeration6LcSubHandleT76SgcEnumerationEntry__ctRC6McHandlePairT79BA9CA_

class gcEnumeration;
class gcEnumerationEntry;

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
    cHandlePairT(const cHandlePairT &src);
};

template <class A, class B>
cHandlePairT<A, B>::cHandlePairT(const cHandlePairT &src) : mHandle(src.mHandle), mSub(src.mSub) {
}

template cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::cHandlePairT(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &);
