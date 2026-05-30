// cRedBlackTree<mVec3, cFixedSizeAllocator<cRedBlackTreeNode<mVec3>, 200>, mVec3,
//   cRedBlackTreeCompare>::FindNode(const cFixedSizeAllocator<...> &) const @ 0x0020bfc8
//
// BST lookup in a red-black tree keyed by mVec3.  The cRedBlackTreeCompare
// functor compares the two mVec3 keys with VFPU triple compares (vcmp.t lt /
// vcmp.t gt) and reads the VFPU condition register ($131).  The combined
// result drives the classic 3-way descent: key<node -> left, node<key ->
// right, equal -> found.
//
// vcmp.t / mfvc have no C equivalent, so the vector compares are inline asm;
// all control flow and the boolean reductions (slt/andi) are plain C so the
// compiler owns the branch/delay-slot scheduling.

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T>
class cRedBlackTreeNode : public T {
public:
    int mParent;
    int mLeft;
    int mRight;
};

template <class T, int N>
class cFixedSizeAllocator {
};

class cRedBlackTreeCompare {
};

struct mVec3 {
    v4sf_t qw;
};

template <class ValueType, class AllocType, class KeyType, class Compare>
class cRedBlackTree {
public:
    cRedBlackTreeNode<ValueType> *mRoot;
    cRedBlackTreeNode<ValueType> *FindNode(const KeyType &key) const;
};

typedef cRedBlackTree<mVec3, cFixedSizeAllocator<cRedBlackTreeNode<mVec3>, 200>,
                      mVec3, cRedBlackTreeCompare>
    Tree;
typedef cRedBlackTreeNode<mVec3> Node;

template <class ValueType, class AllocType, class KeyType, class Compare>
cRedBlackTreeNode<ValueType> *
cRedBlackTree<ValueType, AllocType, KeyType, Compare>::FindNode(const KeyType &key) const
{
    cRedBlackTreeNode<ValueType> *node = mRoot;
    if (node == 0) {
        return 0;
    }
    __asm__ volatile("lv.q C130, 0(%0)\n" : : "r"(&key));
    while (node) {
        int lt, gt;
        __asm__ volatile(
            "lv.q   C120, 0(%2)\n"
            "vcmp.t lt, C130, C120\n"
            "vnop\n"
            "mfvc   %0, $131\n"
            "vcmp.t gt, C130, C120\n"
            "vnop\n"
            "mfvc   %1, $131\n"
            : "=r"(lt), "=r"(gt)
            : "r"(node));

        int cmp;
        int keyLess;
        __asm__ volatile("slt %0, %2, %1\n andi %0, %0, 0xff\n"
                         : "=r"(keyLess) : "r"(lt), "r"(gt));
        if (keyLess) {
            cmp = -1;
        } else {
            int lt2, gt2;
            __asm__ volatile(
                "lv.q   C200, 0(%2)\n"
                "vcmp.t lt, C120, C200\n"
                "vnop\n"
                "mfvc   %0, $131\n"
                "vcmp.t gt, C120, C200\n"
                "vnop\n"
                "mfvc   %1, $131\n"
                : "=r"(lt2), "=r"(gt2)
                : "r"(&key), "r"(node));
            int nodeGreater;
            __asm__ volatile("slt %0, %2, %1\n andi %0, %0, 0xff\n"
                             : "=r"(nodeGreater) : "r"(lt2), "r"(gt2));
            if (nodeGreater) {
                cmp = 1;
                goto descend;
            }
            cmp = 0;
        }

    descend:
        if (cmp < 0) {
            node = (cRedBlackTreeNode<ValueType> *)node->mLeft;
        } else if (cmp > 0) {
            node = (cRedBlackTreeNode<ValueType> *)node->mRight;
        } else {
            return node;
        }
    }
    return 0;
}

template Node *Tree::FindNode(const mVec3 &) const;
