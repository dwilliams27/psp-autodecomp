// cGUIDT<gcRegion>::cGUIDT(void) @ 0x00245578
// Template GUID default constructor: zero two u32 members.
// Emits symbol __0oGcGUIDT76IgcRegion_ctv

class gcRegion;

template <class T>
struct cGUIDT {
    unsigned int mA;
    unsigned int mB;
    cGUIDT();
};

template <class T>
cGUIDT<T>::cGUIDT() : mA(0), mB(0) {}

template cGUIDT<gcRegion>::cGUIDT();
