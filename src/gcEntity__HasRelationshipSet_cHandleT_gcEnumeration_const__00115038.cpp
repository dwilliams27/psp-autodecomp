// gcEntity::HasRelationshipSet(cHandleT<gcEnumeration>) const
// 0x00115038, 152B, gcAll_psp.obj — non-leaf
//
// Resolves this->[0x44] (an enumeration handle) through the global handle
// table at 0x38890 twice: once to validate the candidate (its +0x30 word must
// equal the handle), once to fetch the gcEntityTemplate pointer that is passed
// to gcEntityTemplate::FindRelationshipSetIndex. Returns true iff the index
// the template returns is non-negative.

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
    int FindRelationshipSetIndex(cHandleT<gcEnumeration>) const;
};

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;   // 0x44
    bool HasRelationshipSet(cHandleT<gcEnumeration>) const;
};

bool gcEntity::HasRelationshipSet(cHandleT<gcEnumeration> handle) const {
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
        result = tmpl->FindRelationshipSetIndex(handle) >= 0;
    }

    return result;
}
