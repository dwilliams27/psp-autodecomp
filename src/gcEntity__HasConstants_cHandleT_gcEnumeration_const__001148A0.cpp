// gcEntity::HasConstants(cHandleT<gcEnumeration>) const
// 0x001148a0, 152B, gcAll_psp.obj — non-leaf
//
// Resolves this->mEnumKey through the global enumeration pointer table at
// 0x38890 to get the gcEntityTemplate. If a validated template exists,
// re-derives the template pointer and calls FindConstantsIndex(handle),
// returning whether the index is >= 0.
//
// Byte-exact twin of gcEntity::HasTimeSinceSet (0x114d00) / HasRelationshipSet
// (0x115038); identical codegen modulo the relocated call target.

class gcEnumeration;

template <class T>
class cHandleT {
public:
    int mValue;
};

typedef cHandleT<gcEnumeration> McHandle;

class gcEntityTemplate {
public:
    char _pad30[0x30];
    int mKey;   // 0x30
    int FindConstantsIndex(McHandle) const;
};

// global enumeration pointer table at 0x38890
extern gcEntityTemplate *g_enumTable[];

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;   // 0x44
    int HasConstants(McHandle) const;
};

int gcEntity::HasConstants(McHandle handle) const {
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
        result = node->FindConstantsIndex(handle) >= 0;
    }
    return result;
}
