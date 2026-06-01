// gcUIDialog::IsUpdateEmpty(bool) const @ 0x000E8D58
// Split-TU: declare the types locally so we don't touch shared headers.
//
// Byte-identical structure to the matched sibling gcUIWidget::IsUpdateEmpty
// (0x000DC9C4) apart from the (relocated, masked) call targets:
//   load global manager (0x37D850), index its field 0x4C into a handle table
//   at 0x38890, call gcEnumeration::Get(handle, &local, enumId), then dispatch
//   ProcessesMessage(local) and return whether it returned 0.

template <class A, class B>
struct cSubHandleT {
    int h;
};

class gcEnumeration;
class gcEnumerationEntry;

template <class A, class B>
struct cHandlePairT {
    int a;
    int b;
};

class gcEnumeration {
public:
    static void Get(int, cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry, int> > &, int);
};

class gcUIDialog {
public:
    int ProcessesMessage(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry, int> > &) const;
    int IsUpdateEmpty(bool) const;
};

struct gcManager {
    char pad[0x4C];
    unsigned int field4C;
};

int gcUIDialog::IsUpdateEmpty(bool which) const {
    unsigned int handle = (*(gcManager **)0x37D850)->field4C;
    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry, int> > pairA;
    cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry, int> > pairB;

    if (which) {
        int e = 0;
        if (handle != 0) {
            e = *(int *)(((handle & 0xFFFF) << 2) + 0x38890);
        }
        gcEnumeration::Get(e, pairA, 0x1A);
        return this->ProcessesMessage(pairA) == 0;
    }

    int e = 0;
    if (handle != 0) {
        e = *(int *)(((handle & 0xFFFF) << 2) + 0x38890);
    }
    gcEnumeration::Get(e, pairB, 0x1B);
    return this->ProcessesMessage(pairB) == 0;
}
