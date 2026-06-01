// eDynamicAABBTreeNode<eRigidBodyState>::GetExtents(mBox *)
// Address: 0x0020eaa4, Size: 76B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_KGetExtentsP6EmBox_v
//
// If the cached extents are stale (extentsValid byte at 0x4F == 0), recompute
// them via UpdateExtents(), then copy the node's two stored quadwords (the lo
// and hi corners at 0x00 and 0x10) into the caller's mBox via lv.q/sv.q.
// ODR-safe: types declared locally, never added to a shared header.

typedef int v4sf_t __attribute__((mode(V4SF)));

struct eRigidBodyState;

struct mBox {
    v4sf_t lo;   // 0x00
    v4sf_t hi;   // 0x10
};

template <class T>
class eDynamicAABBTreeNode {
public:
    v4sf_t lo;                        // 0x00
    v4sf_t hi;                        // 0x10
    char pad20[0x4F - 0x20];
    unsigned char extentsValid;       // 0x4F

    void GetExtents(mBox *box);
    void UpdateExtents();
};

#pragma control sched=1

template <class T>
void eDynamicAABBTreeNode<T>::GetExtents(mBox *box) {
    if (extentsValid == 0) {
        UpdateExtents();
    }
    box->lo = lo;
    box->hi = hi;
}

template void eDynamicAABBTreeNode<eRigidBodyState>::GetExtents(mBox *);
