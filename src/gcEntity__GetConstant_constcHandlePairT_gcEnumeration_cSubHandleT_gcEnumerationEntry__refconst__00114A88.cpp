// gcEntity::GetConstant(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const
// 0x00114a88, 144B, gcAll_psp.obj — non-leaf
//
// Calls FindConstant(handle, true, &index, &subHandle); if the resolved
// subHandle is negative, returns 0.0f. Otherwise resolves this->mEnumKey
// through the global enumeration pointer table at 0x38890 to a
// gcEntityTemplate, then indexes its constant tables by the returned index
// and subHandle to load the float constant.

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
    int mFirst;
    int mSecond;
};

typedef cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > McHandlePair;

class gcEntityTemplate;

// global enumeration pointer table at 0x38890
extern gcEntityTemplate *g_enumTable[];

class gcEntity {
public:
    char _padTo44[0x44];
    int mEnumKey;   // 0x44
    float GetConstant(const McHandlePair &handle) const;
    int FindConstant(const McHandlePair &handle, bool create, int *outIndex,
                     const McHandlePair &outSub) const;
};

float gcEntity::GetConstant(const McHandlePair &handle) const {
    int index;
    int sub;

    FindConstant(handle, true, &index, (const McHandlePair &)sub);

    float result;
    if (sub >= 0) {
        char *tmpl = 0;
        int key = mEnumKey;
        if (key != 0) {
            tmpl = (char *)g_enumTable[key & 0xFFFF];
        }

        char *base = *(char **)(tmpl + 0x78);
        char *row = *(char **)(base + index * 4);
        float *vals = *(float **)(row + 0xC);
        result = vals[sub];
    } else {
        result = 0.0f;
    }
    return result;
}
