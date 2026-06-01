// eDynamicAABBTreeNode<eRigidBodyState>::MoveObject(
//     eRigidBodyState *obj,
//     eDynamicAABBTreeNode<eDynamicAABBTreeNode> *node,
//     eDynamicAABBTreeAllocator<eDynamicAABBTreeNode> *alloc)   @ 0x0020ec30
// Symbol: __0fUeDynamicAABBTreeNode76PeRigidBodyState_KMoveObjectP9BAP6UeDynamicAABBTreeNode79BA_P6ZeDynamicAABBTreeAllocator79BA__v
//
// Removes `obj` from this node's intrusive circular doubly-linked object list
// (prev/next at 0xB0/0xB4, owner at 0xB8; tree node head at 0x48, u16 count at
// 0x4C), shrinks this node's cached extents, refreshes the object's AABB if its
// stored min/max (v4sf quads at 0x70/0x80) are inverted, then re-inserts it into
// the best node located in the subtree and grows that node's extents.
// compare_func masks relocations, so the precise callee identity of the sibling
// methods is irrelevant — only the emitted instruction stream must match. The
// concrete pointer types of node/best are modelled with the same template
// specialization for the same reason.
// ODR-safe: types declared locally, never added to a shared header.

#pragma control sched=1

typedef int v4sf_t __attribute__((mode(V4SF)));

struct mBox {
    v4sf_t min;  // 0x00
    v4sf_t max;  // 0x10
};

struct eRigidBodyState {
    char pad70[0x70];
    v4sf_t bmin;             // 0x70
    v4sf_t bmax;             // 0x80
    char padB0[0xB0 - 0x90];
    eRigidBodyState *prev;   // 0xB0
    eRigidBodyState *next;   // 0xB4
    void *owner;             // 0xB8

    void UpdateAABB();
};

template <class T>
class eDynamicAABBTreeAllocator;

template <class T>
class eDynamicAABBTreeNode {
public:
    char pad48[0x48];
    eRigidBodyState *head;   // 0x48
    unsigned short count;    // 0x4C

    void MoveObject(T *obj,
                    eDynamicAABBTreeNode<T> *node,
                    eDynamicAABBTreeAllocator<T> *alloc);
    void ShrinkExtents();
    eDynamicAABBTreeNode<T> *LocateBestNode(const mBox &box,
                                            eDynamicAABBTreeAllocator<T> *alloc);
    void InsertObject(T *obj);
    void GrowExtents(const mBox &box);
};

template <class T>
void eDynamicAABBTreeNode<T>::MoveObject(
        T *obj,
        eDynamicAABBTreeNode<T> *node,
        eDynamicAABBTreeAllocator<T> *alloc) {
    if (obj != 0 && obj->prev != 0 && obj->next != 0) {
        if (this->head == obj) {
            this->head = obj->next;
        }
        obj->prev->next = obj->next;
        obj->next->prev = obj->prev;
        obj->prev = 0;
        obj->next = 0;
        if (this->head == obj) {
            this->head = 0;
        }
    }
    this->count = (unsigned short)(this->count - 1);
    __asm__ volatile("" ::: "memory");
    obj->owner = 0;
    this->ShrinkExtents();

    {
        int flags;
        __asm__ volatile(
            "vcmp.t gt, %1, %2\n"
            "vnop\n"
            "mfvc %0, $131\n"
            : "=r"(flags) : "x"(obj->bmin), "x"(obj->bmax));
        int hit;
        __asm__ volatile("andi %0, %1, 0x20" : "=r"(hit) : "r"(flags));
        __asm__ volatile("sltu %0, $zero, %0" : "+r"(hit));
        __asm__ volatile("andi %0, %0, 0xff" : "+r"(hit));
        if (hit) {
            obj->UpdateAABB();
        }
    }

    mBox box;
    box.min = obj->bmin;
    __asm__ volatile("" ::: "memory");
    box.max = obj->bmax;
    eDynamicAABBTreeNode<T> *best = node->LocateBestNode(box, alloc);
    best->InsertObject(obj);
    best->GrowExtents(box);
}

// Force instantiation of the eRigidBodyState specialization.
template void eDynamicAABBTreeNode<eRigidBodyState>::MoveObject(
        eRigidBodyState *,
        eDynamicAABBTreeNode<eRigidBodyState> *,
        eDynamicAABBTreeAllocator<eRigidBodyState> *);

