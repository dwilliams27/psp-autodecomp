// eDynamicAABBTreeNode<eRigidBodyState>::Balance(eDynamicAABBTreeAllocator<eDynamicAABBTreeNode> *)
// Address: 0x0020ea58, Size: 76B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_HBalanceP6ZeDynamicAABBTreeAllocator79BA__v
//
// Reads the is-internal/locked flag at 0x4E. If set, do nothing. Otherwise, if
// NeedsSplit() reports the node is over the split threshold, SplitNode(alloc).
// Out-of-line bodies for NeedsSplit/SplitNode live in their own TUs; here they
// are external. ODR-safe: types declared locally, never added to a shared header.

#pragma control sched=1

struct eRigidBodyState;

template <class T>
class eDynamicAABBTreeAllocator;

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad48[0x48];
    T *head;                // 0x48
    unsigned short count;   // 0x4C
    unsigned char flag;     // 0x4E

    bool NeedsSplit() const;
    void SplitNode(eDynamicAABBTreeAllocator<T> *alloc);
    void Balance(eDynamicAABBTreeAllocator<T> *alloc);
};

template <class T>
void eDynamicAABBTreeNode<T>::Balance(eDynamicAABBTreeAllocator<T> *alloc) {
    if (this->flag == 0 && NeedsSplit()) {
        SplitNode(alloc);
    }
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::Balance(
    eDynamicAABBTreeAllocator<eRigidBodyState> *);
