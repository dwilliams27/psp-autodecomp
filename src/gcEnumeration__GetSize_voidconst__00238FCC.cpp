// gcEnumeration::GetSize(void) const @ 0x00238fcc
// 200B, gcAll_psp.obj — non-leaf
//
// Structural twin of gcEntity::HasVariables(cHandleT<gcEnumeration>) @ 0x114b18:
// resolves this->mHandle.mId (0x54) through the global handle table at 0x38890
// (D_00038890[id & 0xFFFF]); if the candidate's +0x30 word equals the id, the
// handle aliases another enumeration and GetSize re-resolves and recurses on it.
// Otherwise it asks IsIndexed(): if indexed, the size is mField50 (0x50); else
// it is the count word stored just before the cBaseArray data (mData[-1]) or 0.

extern void *D_00038890[];

class gcEnumeration {
public:
    char _padTo44[0x44];
    int *mArrayData;   // 0x44 (cBaseArray::mData)
    char _pad44[8];    // 0x48..0x4F (mOwner, mField4C, pad)
    int mField50;      // 0x50
    int mHandleId;     // 0x54 (cHandle::mId)

    int GetSize(void) const;
    bool IsIndexed(void) const;
};

// Out-of-line peer accessor: the original re-resolves the handle a second time
// and issues a genuine recursive call (it is NOT turned into a self-loop).
// Routing the recursion through a distinct declaration keeps SNC from collapsing
// the tail call into a backward branch.
int gcEnumeration_GetSize_recurse(const gcEnumeration *);

int gcEnumeration::GetSize(void) const {
    int id = mHandleId;

    void *node;
    void *cand;
    if (id == 0) {
        cand = 0;
        node = 0;
    } else {
        cand = D_00038890[id & 0xFFFF];
        node = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == id) {
            node = cand;
        }
    }

    if (node != 0) {
        gcEnumeration *target = 0;
        if (id != 0) {
            target = (gcEnumeration *)D_00038890[id & 0xFFFF];
        }
        return gcEnumeration_GetSize_recurse(target);
    }

    int result;
    if (IsIndexed()) {
        result = mField50;
    } else {
        int count = 0;
        int *data = mArrayData;
        if (data != 0) {
            count = data[-1];
        }
        result = count;
    }
    return result;
}
