// eDynamicAABBTreeNode<eRigidBodyState>::AddObject(eRigidBodyState *, eDynamicAABBTreeAllocator<eDynamicAABBTreeNode> *)
// Address: 0x0020eaf0, Size: 160B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_JAddObjectP9BAP6ZeDynamicAABBTreeAllocator79BA__v
//
// Compare the object's two stored AABB quadwords (at obj+0x70 / obj+0x80) with
// vcmp.gt.t; if VFPU condition-code bit 5 (the .t aggregate / 0x20 mask) is set
// the AABB is stale, so refresh it via eRigidBodyState::UpdateAABB(). Then copy
// the object's AABB into a stack mBox, locate the best containing node via
// LocateBestNode(), insert the object into that node, and grow the node's
// extents by the object box.
// ODR-safe: types declared locally, never added to a shared header.

typedef int v4sf_t __attribute__((mode(V4SF)));

struct mBox {
    v4sf_t lo;  // 0x00
    v4sf_t hi;  // 0x10
};

struct eRigidBodyState {
    char pad70[0x70];
    v4sf_t aabbLo;  // 0x70
    v4sf_t aabbHi;  // 0x80
    void UpdateAABB();
};

template <class T>
class eDynamicAABBTreeAllocator;

template <class T>
class eDynamicAABBTreeNode {
public:
    eDynamicAABBTreeNode<T> *LocateBestNode(const mBox &box,
                                            eDynamicAABBTreeAllocator<T> *alloc);
    void InsertObject(T *obj);
    void GrowExtents(const mBox &box);
    void AddObject(T *obj, eDynamicAABBTreeAllocator<T> *alloc);
};

template <class T>
void eDynamicAABBTreeNode<T>::AddObject(T *obj, eDynamicAABBTreeAllocator<T> *alloc) {
    int stale;
    __asm__ volatile(
        "lv.q C120, 0x70(%1)\n"
        "lv.q C130, 0x80(%1)\n"
        "vcmp.t gt, C120, C130\n"
        "vnop\n"
        "mfvc %0, $131\n"
        : "=r"(stale)
        : "r"(obj)
        : "memory");
    if (((stale & 0x20) != 0) & 0xFF) {
        obj->UpdateAABB();
    }

    mBox box;
    box.lo = obj->aabbLo;
    __asm__ volatile("" ::: "memory");
    box.hi = obj->aabbHi;

    eDynamicAABBTreeNode<T> *best = LocateBestNode(box, alloc);
    best->InsertObject(obj);
    best->GrowExtents(box);
}

template void eDynamicAABBTreeNode<eRigidBodyState>::AddObject(
    eRigidBodyState *, eDynamicAABBTreeAllocator<eRigidBodyState> *);
