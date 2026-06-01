// gcEntity::HasVariables(cHandleT<gcEnumeration>) const @ 0x00114b18
// 152B, gcAll_psp.obj — non-leaf
//
// Structural twin of gcEntity::HasRelationshipSet (0x115038) and
// HasTimeSinceSet (0x114d00). Resolves this->[0x44] (an enumeration handle)
// through the global handle table at 0x38890 twice: once to validate the
// candidate (its +0x30 word must equal the handle), once to re-fetch the
// gcEntityTemplate pointer passed to FindVariablesIndex. Returns true iff the
// returned index is non-negative.

class gcEnumeration;
template <class A> class cHandleT;

template <class A>
class cHandleT {
public:
    int mIndex;
};

extern void *D_00038890[];

class gcEntityTemplate {
public:
    int FindVariablesIndex(cHandleT<gcEnumeration>) const;
};

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;   // 0x44
    bool HasVariables(cHandleT<gcEnumeration>) const;
};

bool gcEntity::HasVariables(cHandleT<gcEnumeration> handle) const {
    bool result = false;
    int key = mEnumKey;

    void *node;
    void *cand;
    if (key == 0) {
        cand = 0;
        node = 0;
    } else {
        cand = D_00038890[key & 0xFFFF];
        node = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == key) {
            node = cand;
        }
    }

    if (node != 0) {
        gcEntityTemplate *tmpl = 0;
        if (key != 0) {
            tmpl = (gcEntityTemplate *)D_00038890[key & 0xFFFF];
        }
        result = tmpl->FindVariablesIndex(handle) >= 0;
    }

    return result;
}
