// 0x002275f0 cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::cHandlePairT(void)
// Default constructor: zero two sentinel ints, return this.
// Symbol: __0oMcHandlePairT76NgcEnumeration6LcSubHandleT76SgcEnumerationEntry__ctv

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
    cHandlePairT();
};

template <class A, class B>
cHandlePairT<A, B>::cHandlePairT() : mHandle(0), mSub(0) {
}

template cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> >::cHandlePairT();
