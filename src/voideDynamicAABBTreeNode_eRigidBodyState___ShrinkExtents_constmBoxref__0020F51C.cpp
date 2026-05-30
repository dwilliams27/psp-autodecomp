// eDynamicAABBTreeNode<eRigidBodyState>::ShrinkExtents(const mBox &)
// Address: 0x0020f51c, Size: 176B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_NShrinkExtentsRC6EmBox_v
//
// Walk up the parent chain (link at 0x34). On each ancestor, clear the
// "extents valid" byte flag at 0x4F unless the box strictly contains the
// node's stored extents. The short-circuit OR of six float comparisons
// produces the bc1tl/bc1fl branch-likely chain, each with the clearing
// store (sb zero) in its delay slot; the trailing bnezl preloads the next
// iteration's first compare.
// ODR-safe: types declared locally, never added to a shared header.

struct eRigidBodyState;

struct mBox {
    float min[4];  // 0x00 .. 0x0C
    float max[4];  // 0x10 .. 0x1C
};

template <class T>
class eDynamicAABBTreeNode {
public:
    float lo0;                        // 0x00
    float lo4;                        // 0x04
    float lo8;                        // 0x08
    char pad0C[0x10 - 0x0C];
    float hi10;                       // 0x10
    float hi14;                       // 0x14
    float hi18;                       // 0x18
    char pad1C[0x34 - 0x1C];
    eDynamicAABBTreeNode<T> *parent;  // 0x34
    char pad38[0x4F - 0x38];
    char extentsValid;                // 0x4F

    void ShrinkExtents(const mBox &box);
};

template <class T>
void eDynamicAABBTreeNode<T>::ShrinkExtents(const mBox &box) {
    eDynamicAABBTreeNode<T> *node = this;
    if (node != 0) {
        do {
            if (box.min[0] <= node->lo0 ||
                box.min[1] <= node->lo4 ||
                box.min[2] <= node->lo8 ||
                !(box.max[0] < node->hi10) ||
                !(box.max[1] < node->hi14) ||
                !(box.max[2] < node->hi18)) {
                node->extentsValid = 0;
            }
            node = node->parent;
        } while (node != 0);
    }
}

template void eDynamicAABBTreeNode<eRigidBodyState>::ShrinkExtents(const mBox &);
