class gcStringTable;
class gcString;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

class gcStringValue {
public:
    void Get(cHandlePairT<gcStringTable, cSubHandleT<gcString> > *) const;
};

void gcStringValue::Get(cHandlePairT<gcStringTable, cSubHandleT<gcString> > *out) const {
    out->mHandle.mIndex = 0;
    cSubHandleT<gcString> *p = &out->mSubHandle;
    p->mIndex = 0;
}
