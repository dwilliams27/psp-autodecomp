// eDynamicAABBTreeNode<eRigidBodyState>::ShrinkExtents(void)
// Address: 0x0020f5cc, Size: 32B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_NShrinkExtentsv_v
//
// Walk up the parent chain (link at 0x34) clearing the "extents valid" byte
// flag at 0x4F on each ancestor. Loop with preloaded next iteration produces
// the bnezl branch-likely with the store in the delay slot.
// ODR-safe: type declared locally, never added to a shared header.

struct eRigidBodyState;

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad34[0x34];
    eDynamicAABBTreeNode<T> *parent;  // 0x34
    char pad38[0x4F - 0x38];
    char extentsValid;                // 0x4F

    void ShrinkExtents();
};

template <class T>
void eDynamicAABBTreeNode<T>::ShrinkExtents() {
    eDynamicAABBTreeNode<T> *node = this;
    if (node != 0) {
        do {
            node->extentsValid = 0;
            __asm__ volatile("" ::: "memory");
            node = node->parent;
        } while (node != 0);
    }
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::ShrinkExtents();
