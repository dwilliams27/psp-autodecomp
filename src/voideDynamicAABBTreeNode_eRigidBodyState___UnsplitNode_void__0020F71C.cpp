// eDynamicAABBTreeNode<eRigidBodyState>::UnsplitNode(void)
// Address: 0x0020f71c, Size: 36B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_LUnsplitNodev_v
//
// Clears the 3 child-slot words at offset 0x3C..0x44 in a forward loop walking
// a separate cursor pointer, then clears a u8 split-flag at offset 0x4E.
// ODR-safe: types declared locally, never added to a shared header.

struct eRigidBodyState;

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad3C[0x3C];
    int children[3];        // 0x3C, 0x40, 0x44
    char pad48[0x4E - 0x48];
    unsigned char isSplit;  // 0x4E

    void UnsplitNode();
};

template <class T>
void eDynamicAABBTreeNode<T>::UnsplitNode() {
    int i = 0;
    eDynamicAABBTreeNode<T> *p = this;
    do {
        p->children[0] = 0;
        ++i;
        p = (eDynamicAABBTreeNode<T> *)((char *)p + 4);
    } while (i < 3);
    this->isSplit = 0;
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::UnsplitNode();
