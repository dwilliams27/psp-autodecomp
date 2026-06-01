// eDynamicAABBTreeNode<eRigidBodyState>::RemoveChild(eDynamicAABBTreeNode<...> *, eDynamicAABBTreeAllocator<...> *, bool)
// Address: 0x0020f410, Size: 188B, Obj: eAll_psp.obj
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_LRemoveChildP6UeDynamicAABBTreeNode79BA_P6ZeDynamicAABBTreeAllocator79BA_b_v
//
// Walks the three child slots (members at 0x3c/0x40/0x44, addressed by byte
// arithmetic ((char*)this + i*4)->0x3c). When a slot equals the child being
// removed, that node is freed back to the allocator and the slot cleared; any
// other non-null slot clears the "all children empty" flag. After the sweep, an
// all-empty node is collapsed via UnsplitNode, and (when the bool is set) the
// node is rechecked for emptiness via CheckEmpty. ODR-safe: types declared
// locally, never added to a shared header.

#pragma control sched=1

struct eRigidBodyState;

template <class T> class eDynamicAABBTreeNode;

template <class T>
class eDynamicAABBTreeAllocator {
public:
    void FreeNode(eDynamicAABBTreeNode<T> *node);
};

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad3c[0x3c];
    eDynamicAABBTreeNode<T> *child;   // 0x3c

    void UnsplitNode();
    void CheckEmpty(eDynamicAABBTreeAllocator<T> *alloc);
    void RemoveChild(eDynamicAABBTreeNode<T> *node,
                     eDynamicAABBTreeAllocator<T> *alloc, bool check);
};

template <class T>
void eDynamicAABBTreeNode<T>::RemoveChild(eDynamicAABBTreeNode<T> *node,
                                          eDynamicAABBTreeAllocator<T> *alloc,
                                          bool check) {
    bool allEmpty = true;
    int i = 0;
    eDynamicAABBTreeNode<T> *slot = this;
    do {
        eDynamicAABBTreeNode<T> *c = slot->child;
        if (c == node) {
            __asm__ volatile("" ::: "memory");
            alloc->FreeNode(c);
            slot->child = 0;
        } else if (c != 0) {
            allEmpty = false;
        }
        i++;
        slot = (eDynamicAABBTreeNode<T> *)((char *)slot + 4);
    } while (i < 3);

    if (allEmpty) {
        this->UnsplitNode();
    }
    if (check) {
        this->CheckEmpty(alloc);
    }
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::RemoveChild(
    eDynamicAABBTreeNode<eRigidBodyState> *,
    eDynamicAABBTreeAllocator<eRigidBodyState> *, bool);
