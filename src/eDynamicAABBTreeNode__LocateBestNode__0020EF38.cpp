// eDynamicAABBTreeNode<eRigidBodyState>::LocateBestNode(const mBox&, eDynamicAABBTreeAllocator<eDynamicAABBTreeNode>*)
// Address: 0x0020ef38, Size: 120B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_OLocateBestNodeRC6EmBoxP6ZeDynamicAABBTreeAllocator79BA__P6UeDynamicAABBTreeNode79BA_
//
// Walk down the tree: from this node, ask WhichChild(box) which child a box
// belongs in. While that index is >= 0, descend into (allocating if needed)
// that child via GetOrAllocChild, and re-test. Return the deepest node found.
// ODR-safe: types declared locally, never added to a shared header.

#pragma control sched=1

struct eRigidBodyState;
class mBox;

template <class T> class eDynamicAABBTreeNode;

template <class T>
class eDynamicAABBTreeAllocator {
public:
    eDynamicAABBTreeNode<T> *AllocNode(eDynamicAABBTreeNode<T> *node);
};

template <class T>
class eDynamicAABBTreeNode {
public:
    int WhichChild(const mBox &box);
    eDynamicAABBTreeNode<T> *GetOrAllocChild(int index,
                                             eDynamicAABBTreeAllocator<T> *alloc);
    eDynamicAABBTreeNode<T> *LocateBestNode(const mBox &box,
                                            eDynamicAABBTreeAllocator<T> *alloc);
};

template <class T>
eDynamicAABBTreeNode<T> *
eDynamicAABBTreeNode<T>::LocateBestNode(const mBox &box,
                                       eDynamicAABBTreeAllocator<T> *alloc) {
    __asm__ volatile("" ::: "memory");
    eDynamicAABBTreeNode<T> *node = this;
    int which = node->WhichChild(box);
    while (which >= 0) {
        node = node->GetOrAllocChild(which, alloc);
        which = node->WhichChild(box);
    }
    return node;
}

template eDynamicAABBTreeNode<eRigidBodyState> *
eDynamicAABBTreeNode<eRigidBodyState>::LocateBestNode(
    const mBox &, eDynamicAABBTreeAllocator<eRigidBodyState> *);
