// eDynamicAABBTreeNode<eRigidBodyState>::GrowExtents(const mBox &)
// Address: 0x0020f4cc, Size: 80B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_LGrowExtentsRC6EmBox_v
//
// Walk up the parent chain (link at 0x34). On each ancestor whose
// "extents valid" byte flag at 0x4F is set, extend the node's stored mBox
// (at offset 0x00) by the argument box via mBox::Extend(const mBox&).
// Pattern-4 loop: the trailing bnezl preloads the next iteration's flag
// load (lbu) into its delay slot.
// ODR-safe: types declared locally, never added to a shared header.

struct eRigidBodyState;

struct mBox {
    float min[4];  // 0x00 .. 0x0C
    float max[4];  // 0x10 .. 0x1C
    void Extend(const mBox &box);
};

template <class T>
class eDynamicAABBTreeNode {
public:
    mBox box;                         // 0x00 .. 0x1F
    char pad20[0x34 - 0x20];
    eDynamicAABBTreeNode<T> *parent;  // 0x34
    char pad38[0x4F - 0x38];
    unsigned char extentsValid;       // 0x4F

    void GrowExtents(const mBox &box);
};

#pragma control sched=1
template <class T>
void eDynamicAABBTreeNode<T>::GrowExtents(const mBox &box) {
    eDynamicAABBTreeNode<T> *node = this;
    if (node != 0) {
        do {
            if (node->extentsValid != 0) {
                node->box.Extend(box);
            }
            node = node->parent;
        } while (node != 0);
    }
}

template void eDynamicAABBTreeNode<eRigidBodyState>::GrowExtents(const mBox &);
