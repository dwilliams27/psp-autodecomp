// eDynamicAABBTreeNode<eRigidBodyState>::GetOrAllocChild(int, eDynamicAABBTreeAllocator<eDynamicAABBTreeNode>*)
// Address: 0x0020f1a0, Size: 96B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_PGetOrAllocChildiP6ZeDynamicAABBTreeAllocator79BA__P6UeDynamicAABBTreeNode79BA_
//
// Returns the index'th child node, allocating it lazily through the
// allocator when the slot (member at 0x3c of the index-selected node) is
// null. A negative index short-circuits and returns this.
// The child slot is reached by byte arithmetic: ((char*)this + index*4)->0x3c.
// On a failed allocation the original this is returned.
// ODR-safe: types declared locally, never added to a shared header.

struct eRigidBodyState;

template <class T> class eDynamicAABBTreeNode;

template <class T>
class eDynamicAABBTreeAllocator {
public:
    eDynamicAABBTreeNode<T> *AllocNode(eDynamicAABBTreeNode<T> *node);
};

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad3c[0x3c];
    eDynamicAABBTreeNode<T> *child;   // 0x3c

    eDynamicAABBTreeNode<T> *GetOrAllocChild(int index,
                                             eDynamicAABBTreeAllocator<T> *alloc);
};

template <class T>
eDynamicAABBTreeNode<T> *
eDynamicAABBTreeNode<T>::GetOrAllocChild(int index,
                                        eDynamicAABBTreeAllocator<T> *alloc) {
    if (index < 0) {
        return this;
    }
    eDynamicAABBTreeNode<T> *slot =
        (eDynamicAABBTreeNode<T> *)((char *)this + index * 4);
    eDynamicAABBTreeNode<T> *c = slot->child;
    if (c == 0) {
        __asm__ volatile("" ::: "memory");
        c = alloc->AllocNode(this);
        slot->child = c;
        if (c == 0) {
            return this;
        }
    }
    return c;
}

template eDynamicAABBTreeNode<eRigidBodyState> *
eDynamicAABBTreeNode<eRigidBodyState>::GetOrAllocChild(
    int, eDynamicAABBTreeAllocator<eRigidBodyState> *);
