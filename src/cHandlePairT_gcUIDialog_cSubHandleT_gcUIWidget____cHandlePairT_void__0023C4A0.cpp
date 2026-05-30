// 0x0023c4a0 cHandlePairT<gcUIDialog, cSubHandleT<gcUIWidget> >::cHandlePairT(void)
// Default constructor: zero two sentinel ints, return this.
// Symbol: __0oMcHandlePairT76KgcUIDialog6LcSubHandleT76KgcUIWidget__ctv

class gcUIDialog;
class gcUIWidget;

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

template cHandlePairT<gcUIDialog, cSubHandleT<gcUIWidget> >::cHandlePairT();
