// gcEnumeration::IsEntryReplicated(int) const @ 0x000d4a8c
// 280B, gcAll_psp.obj — non-leaf
//
// Structural twin of gcEnumeration::GetSize @ 0x238fcc:
// resolves this->mHandleId (0x54) through the global handle table at 0x38890
// (D_00038890[id & 0xFFFF]); if the candidate's +0x30 word equals the id, the
// handle aliases another enumeration and the call re-resolves and recurses on it
// (passing the same index argument). Otherwise: if IsIndexed() is true the answer
// is HasReplicatedEntries(); else look up mArrayData[index] (0x44) and return
// whether the entry's +0x20 byte has bit 0 set.

extern void *D_00038890[];

class gcEnumeration {
public:
    char _padTo44[0x44];
    void **mArrayData;   // 0x44 (cBaseArray::mData)
    char _pad44[0xC];    // 0x48..0x53
    int mHandleId;       // 0x54 (cHandle::mId)

    unsigned char IsEntryReplicated(int index) const;
    unsigned char IsIndexed(void) const;
    unsigned char HasReplicatedEntries(void) const;
};

// Out-of-line peer: the original re-resolves the handle a second time and issues
// a genuine recursive call (not a self-loop). Routing through a distinct
// declaration keeps SNC from collapsing the tail call into a backward branch.
unsigned char gcEnumeration_IsEntryReplicated_recurse(const gcEnumeration *, int);

unsigned char gcEnumeration::IsEntryReplicated(int index) const {
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
        return gcEnumeration_IsEntryReplicated_recurse(target, index);
    }

    if (IsIndexed()) {
        return HasReplicatedEntries();
    }

    void *entry = mArrayData[index];
    int result = 0;
    if (entry != 0 && (*(unsigned char *)((char *)entry + 0x20) & 1)) {
        result = 1;
    }
    return result;
}
