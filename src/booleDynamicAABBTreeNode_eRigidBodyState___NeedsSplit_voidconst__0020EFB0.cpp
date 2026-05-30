// eDynamicAABBTreeNode<eRigidBodyState>::NeedsSplit(void) const
// Address: 0x0020efb0, Size: 36B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_KNeedsSplitv_bK
//
// Leaf predicate. lbu flag at 0x4E (is-internal/locked); if set the node never
// needs to split. Otherwise compare the u16 object count at 0x4C against the
// split threshold (4): NeedsSplit == count >= 4. ODR-safe: types declared
// locally, never added to a shared header.

struct eRigidBodyState;

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad48[0x48];
    T *head;                // 0x48
    unsigned short count;   // 0x4C
    unsigned char flag;     // 0x4E

    bool NeedsSplit() const;
};

template <class T>
bool eDynamicAABBTreeNode<T>::NeedsSplit() const {
    if (this->flag != 0) {
        return false;
    }
    return this->count >= 4;
}

// Force instantiation of the eRigidBodyState specialization.
template bool eDynamicAABBTreeNode<eRigidBodyState>::NeedsSplit() const;
