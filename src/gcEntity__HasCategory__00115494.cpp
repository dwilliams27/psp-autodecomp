// gcEntity::HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const
// 0x00115494, 148B, gcAll_psp.obj — non-leaf
//
// Sibling of HasTimeSinceSet (0x114d00) / HasRelationshipSet (0x115038):
// resolves this->mEnumKey through the global enumeration pointer table at
// 0x38890 twice (validate via +0x30 == key, then re-derive the template), then
// tail-calls gcEntityTemplate::HasCategory(handle), returning its result
// directly (no >= 0 fold — the callee already returns the category bool/int).

class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cSubHandleT {
public:
    int mValue;
};

template <class A, class B>
class cHandlePairT {
public:
    int mValue;
};

typedef cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > McHandlePair;

class gcEntityTemplate {
public:
    char _pad30[0x30];
    int mKey;   // 0x30
    int HasCategory(const McHandlePair &) const;
};

// global enumeration pointer table at 0x38890
extern gcEntityTemplate *g_enumTable[];

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;   // 0x44
    int HasCategory(const McHandlePair &) const;
};

int gcEntity::HasCategory(const McHandlePair &handle) const {
    int result = 0;
    int key = mEnumKey;

    gcEntityTemplate *tmpl;
    if (key == 0) {
        tmpl = 0;
    } else {
        unsigned int rt56882 = key & 0xFFFF;
        gcEntityTemplate *node = g_enumTable[rt56882];
        tmpl = (node != 0 && node->mKey == key) ? node : 0;
    }

    if (tmpl != 0) {
        gcEntityTemplate *node = 0;
        if (key != 0) {
            unsigned int rt91731 = key & 0xFFFF;
            node = g_enumTable[rt91731];
        }
        result = node->HasCategory(handle);
    }
    return result;
}
