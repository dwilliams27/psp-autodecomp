// eDynamicAABBTreeAllocator<eRigidBodyState>::AllocNode(eDynamicAABBTreeNode<...> *)
// Address: 0x0020a970, Size: 124B, Obj: eAll_psp.obj
// Symbol: __0fZeDynamicAABBTreeAllocator76PeRigidBodyState_JAllocNodeP6UeDynamicAABBTreeNode79BA__P6UeDynamicAABBTreeNode79BA_
//
// Pops a node off the allocator's free list. If the free list is empty,
// returns NULL. Otherwise unlinks the head (free_head = head->next), records
// the object pointer in the node, calls UnsplitNode() on it, clears a u8 flag,
// decrements the free count, and returns the node.
// ODR-safe: types declared locally, never added to a shared header.

class eRigidBodyState;

template <class T>
class eDynamicAABBTreeNode {
public:
    char  pad00[0x34];
    void *unk34;     // 0x34  -> stored object pointer
    eDynamicAABBTreeNode<T> *unk38; // 0x38  -> next free node
    char  pad3c[0x13];
    unsigned char unk4F; // 0x4F

    void UnsplitNode();
};

template <class T>
class eDynamicAABBTreeAllocator {
public:
    char  pad00[0x4];
    eDynamicAABBTreeNode<T> *free_head; // 0x04
    unsigned int   free_count;                   // 0x08

    eDynamicAABBTreeNode<T> *AllocNode(eDynamicAABBTreeNode<T> *obj);
};

#pragma control sched=1
template <class T>
eDynamicAABBTreeNode<T> *eDynamicAABBTreeAllocator<T>::AllocNode(
    eDynamicAABBTreeNode<T> *obj)
{
    __asm__ volatile("" ::: "memory");
    if (this->free_head) {
        eDynamicAABBTreeNode<T> *node = this->free_head;
        this->free_head = node->unk38;
        __asm__ volatile("" ::: "memory");
        node->unk34 = obj;
        node->UnsplitNode();
        node->unk4F = 0;
        this->free_count = this->free_count - 1;
        __asm__ volatile("" ::: "memory");
        return node;
    }
    return 0;
}

// Force instantiation of the eRigidBodyState specialization.
template eDynamicAABBTreeNode<eRigidBodyState> *
eDynamicAABBTreeAllocator<eRigidBodyState>::AllocNode(
    eDynamicAABBTreeNode<eRigidBodyState> *);
