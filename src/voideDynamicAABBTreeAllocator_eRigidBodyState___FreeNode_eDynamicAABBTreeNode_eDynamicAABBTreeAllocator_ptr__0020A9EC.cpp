// eDynamicAABBTreeAllocator<eRigidBodyState>::FreeNode(eDynamicAABBTreeNode<...> *)
// Address: 0x0020a9ec, Size: 164B, Obj: eAll_psp.obj
// Symbol: __0fZeDynamicAABBTreeAllocator76PeRigidBodyState_IFreeNodeP6UeDynamicAABBTreeNode79BA__v
//
// Frees a tree node back to the allocator's free list. Re-initializes the
// node's AABB to the "empty" sentinel (min = +FLT_MAX, max = -FLT_MAX),
// zeroes its center/extent quad and the bookkeeping fields, then links the
// node onto the allocator's free list, incrementing the free count.
//
// VFPU codegen: the inlined eDynamicAABBTreeNode constructor zeroes a vec3
// temp (vzero.t C120), reuses column C120 for the FLT_MAX / -FLT_MAX bounds
// and stores them with sv.q. No sv.q lands in the jr-ra delay slot, so the
// VFPU work is pure inline asm; the surrounding scalar stores and free-list
// links are plain C. ODR-safe: types declared locally, never shared headers.

template <class T>
class eDynamicAABBTreeNode {
public:
    // 0x00 min  (vec3, w pad)  -> +FLT_MAX
    // 0x10 max  (vec3, w pad)  -> -FLT_MAX
    // 0x20 zero quad
    char     pad00[0x30];
    int      unk30;       // 0x30
    int      unk34;       // 0x34
    void    *unk38;       // 0x38  -> previous free_head
    char     pad3c[0x12];
    unsigned char unk4E;  // 0x4E
    unsigned char unk4F;  // 0x4F
};

class eRigidBodyState;

template <class T>
class eDynamicAABBTreeAllocator {
public:
    char  pad00[0x4];
    eDynamicAABBTreeNode<T> *free_head; // 0x04
    int   free_count;                   // 0x08

    void FreeNode(eDynamicAABBTreeNode<T> *node);
};

template <class T>
void eDynamicAABBTreeAllocator<T>::FreeNode(
    eDynamicAABBTreeNode<T> *node)
{
    // 16-byte stack scratch the inlined vec3 ctor writes its zero temp to.
    char scratch[16];

    // Inlined node-constructor's empty loop over the 3 vector components.
    int i = 0;
    do {
        i++;
        __asm__ volatile("" ::: "memory");
    } while (i < 3);

    node->unk34 = 0;
    node->unk4E = 0;
    node->unk30 = 0;

    // vzero.t C120 -> store zero quad to scratch + 0x20(node).
    __asm__ volatile(
        "vzero.t C120\n"
        "sv.q C120, 0(%0)\n"
        "sv.q C120, 0x20(%1)\n"
        :: "r"(scratch), "r"(node) : "memory"
    );

    // min = (+FLT_MAX, +FLT_MAX, +FLT_MAX); w component left as the prior zero.
    {
        float fmax = 3.4028234663852886e+38f; // 0x7f7fffff
        int ia, ib, ic;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ia) : "f"(fmax));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ib) : "f"(fmax));
        __asm__ volatile("mfc1 %0, %1" : "=r"(ic) : "f"(fmax));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0(%3)\n"
            :: "r"(ia), "r"(ib), "r"(ic), "r"(node) : "memory"
        );
    }

    // max = (-FLT_MAX, -FLT_MAX, -FLT_MAX).
    {
        float fmin = -3.4028234663852886e+38f; // 0xff7fffff
        int ja, jb, jc;
        __asm__ volatile("mfc1 %0, %1" : "=r"(ja) : "f"(fmin));
        __asm__ volatile("mfc1 %0, %1" : "=r"(jb) : "f"(fmin));
        __asm__ volatile("mfc1 %0, %1" : "=r"(jc) : "f"(fmin));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x10(%3)\n"
            :: "r"(ja), "r"(jb), "r"(jc), "r"(node) : "memory"
        );
    }

    node->unk4F = 0;
    node->unk38 = this->free_head;
    this->free_count = this->free_count + 1;
    this->free_head = node;
}

// Force instantiation of the eRigidBodyState specialization.
template void
eDynamicAABBTreeAllocator<eRigidBodyState>::FreeNode(
    eDynamicAABBTreeNode<eRigidBodyState> *);
