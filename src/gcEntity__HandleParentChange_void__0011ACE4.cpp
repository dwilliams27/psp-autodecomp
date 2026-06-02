// gcEntity::HandleParentChange(void)
// Address: 0x0011ace4, Size: 268B
// Obj: gcAll_psp.obj
// mangled: __0fIgcEntitySHandleParentChangev
//
// If this entity has a parent (+0x218) and is not itself flagged with 0x4
// (+0x5C), copy two flag bits (0xC00) plus two words (+0x74/+0x78) down from
// the parent. Then walk the circular sibling list rooted at +0x21C (chained
// through +0x224), collecting up to 1000 children onto a stack array, and
// recurse into each one whose own parent (+0x218) is this entity.

class gcEntity {
public:
    char _pad0[0x5C];
    int mFlags;        // 0x5C
    char _pad60[0x74 - 0x60];
    int m74;           // 0x74
    int m78;           // 0x78
    char _pad7C[0x218 - 0x7C];
    gcEntity *mParent; // 0x218
    gcEntity *mChild;  // 0x21C
    char _pad220[0x224 - 0x220];
    gcEntity *mSibling;// 0x224

    bool TestFlag() const { return (mFlags & 4) != 0; }
    void HandleParentChange();
};

void gcEntity::HandleParentChange() {
    gcEntity *parent = mParent;
    if (parent != 0 && !TestFlag()) {
        int flags = mFlags & ~0xC00;
        mFlags = flags;
        int *psrc = (int *)((char *)parent + 0x74);
        mFlags = flags | (parent->mFlags & 0xC00);
        int *pdst = (int *)((char *)this + 0x74);
        int *qdst = (int *)((char *)this + 0x78);
        *pdst = *psrc;
        int *qsrc = (int *)((char *)mParent + 0x78);
        *qdst = *qsrc;
    }

    gcEntity *head = mChild;
    gcEntity *node = head;
    int count = 0;
    if (node != 0) {
        gcEntity *stack[1000];
        do {
            stack[count] = node;
            node = node->mSibling;
            unsigned int next = count + 1;
            count = next;
        } while (node != head && count < 1000);

        for (int i = 0; i < count; i++) {
            gcEntity *child = stack[i];
            if (child->mParent == this) {
                child->HandleParentChange();
            }
        }
    }
}
