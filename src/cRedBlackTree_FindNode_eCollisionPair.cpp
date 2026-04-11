// cRedBlackTree<eCollisionPair, ...>::FindNode
// Address: 0x001e5bc4, Size: 500B, Obj: eAll_psp.obj
//
// BST lookup in a red-black tree keyed by eCollisionPair.
// Comparison is lexicographic over 5 unsigned fields at offsets 0x10, 0x14, 0x18, 0x38, 0x3C.
// First compares key < node, then node < key. If neither, node is found.
// Left child at node+0x64, right child at node+0x68.
__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fNcRedBlackTree76OeCollisionPair6XcFixedSizeAllocatorBase76RcRedBlackTreeNode76OeCollisionPair__6OeCollisionPair6UcRedBlackTreeCompare_IFindNodeRC9CA_P6RcRedBlackTreeNode79DA_K\n"
    "__0fNcRedBlackTree76OeCollisionPair6XcFixedSizeAllocatorBase76RcRedBlackTreeNode76OeCollisionPair__6OeCollisionPair6UcRedBlackTreeCompare_IFindNodeRC9CA_P6RcRedBlackTreeNode79DA_K:\n"

    // --- Prologue: load root, null check, cache key[0x10] ---
    ".word 0x8c820000\n"             // lw v0, 0(a0)           — node = this->mRoot
    ".word 0x10400079\n"             // beqz v0, return_null
    ".word 0x00000000\n"             // nop
    ".word 0x8ca40010\n"             // lw a0, 16(a1)          — a0 = key[0x10] (loop-invariant)

    // --- First comparison: key < node (result in a3) ---
    // Field 0x10
    ".word 0x8c460010\n"             // lw a2, 16(v0)          — a2 = node[0x10]
    ".word 0x0086382b\n"             // sltu a3, a0, a2        — key < node?
    ".word 0x10e00003\n"             // beqz a3, +3
    ".word 0x00c4382b\n"             // sltu a3, a2, a0        — (delay) node < key?
    ".word 0x1000002d\n"             // b cmp1_end
    ".word 0x34070001\n"             // li a3, 1               — (delay) key < node → true

    // Field 0x14
    ".word 0x50e00003\n"             // beqzl a3, +3           — if equal, check next
    ".word 0x8ca80014\n"             // lw t0, 20(a1)          — (delay) t0 = key[0x14]
    ".word 0x10000029\n"             // b cmp1_end
    ".word 0x34070000\n"             // li a3, 0               — (delay) node < key → false

    ".word 0x8c470014\n"             // lw a3, 20(v0)          — a3 = node[0x14]
    ".word 0x0107482b\n"             // sltu t1, t0, a3        — key < node?
    ".word 0x11200003\n"             // beqz t1, +3
    ".word 0x00e8382b\n"             // sltu a3, a3, t0        — (delay) node < key?
    ".word 0x10000023\n"             // b cmp1_end
    ".word 0x34070001\n"             // li a3, 1

    // Field 0x18
    ".word 0x50e00003\n"             // beqzl a3, +3
    ".word 0x8ca80018\n"             // lw t0, 24(a1)
    ".word 0x1000001f\n"             // b cmp1_end
    ".word 0x34070000\n"             // li a3, 0

    ".word 0x8c470018\n"             // lw a3, 24(v0)
    ".word 0x0107482b\n"             // sltu t1, t0, a3
    ".word 0x11200003\n"             // beqz t1, +3
    ".word 0x00e8382b\n"             // sltu a3, a3, t0
    ".word 0x10000019\n"             // b cmp1_end
    ".word 0x34070001\n"             // li a3, 1

    // Field 0x38
    ".word 0x50e00003\n"             // beqzl a3, +3
    ".word 0x8ca80038\n"             // lw t0, 56(a1)
    ".word 0x10000015\n"             // b cmp1_end
    ".word 0x34070000\n"             // li a3, 0

    ".word 0x8c470038\n"             // lw a3, 56(v0)
    ".word 0x0107482b\n"             // sltu t1, t0, a3
    ".word 0x11200003\n"             // beqz t1, +3
    ".word 0x00e8382b\n"             // sltu a3, a3, t0
    ".word 0x1000000f\n"             // b cmp1_end
    ".word 0x34070001\n"             // li a3, 1

    // Field 0x3C (last)
    ".word 0x50e00003\n"             // beqzl a3, +3
    ".word 0x8ca8003c\n"             // lw t0, 60(a1)
    ".word 0x1000000b\n"             // b cmp1_end
    ".word 0x34070000\n"             // li a3, 0

    ".word 0x8c47003c\n"             // lw a3, 60(v0)
    ".word 0x0107482b\n"             // sltu t1, t0, a3
    ".word 0x11200003\n"             // beqz t1, +3
    ".word 0x00e8382b\n"             // sltu a3, a3, t0
    ".word 0x10000005\n"             // b cmp1_end
    ".word 0x34070001\n"             // li a3, 1

    ".word 0x10e00003\n"             // beqz a3, cmp1_end      — last field: both paths → 0
    ".word 0x34070000\n"             // li a3, 0
    ".word 0x10000001\n"             // b cmp1_end
    ".word 0x34070000\n"             // li a3, 0

    // --- cmp1_end: dispatch on first comparison result ---
    ".word 0x50e00003\n"             // beqzl a3, cmp2_start   — key NOT < node, check reverse
    ".word 0x8ca70010\n"             // lw a3, 16(a1)          — (delay) a3 = key[0x10]
    ".word 0x10000036\n"             // b direction             — key < node
    ".word 0x2406ffff\n"             // li a2, -1              — (delay) result = -1

    // --- Second comparison: node < key (result in a2) ---
    // Field 0x10 (a2 = node[0x10] still cached, a3 = key[0x10] just loaded)
    ".word 0x00c7402b\n"             // sltu t0, a2, a3        — node < key?
    ".word 0x11000003\n"             // beqz t0, +3
    ".word 0x00e6302b\n"             // sltu a2, a3, a2        — (delay) key < node?
    ".word 0x1000002d\n"             // b cmp2_end
    ".word 0x34060001\n"             // li a2, 1

    // Field 0x14
    ".word 0x50c00003\n"             // beqzl a2, +3
    ".word 0x8c470014\n"             // lw a3, 20(v0)          — a3 = node[0x14]
    ".word 0x10000029\n"             // b cmp2_end
    ".word 0x34060000\n"             // li a2, 0

    ".word 0x8ca60014\n"             // lw a2, 20(a1)          — a2 = key[0x14]
    ".word 0x00e6402b\n"             // sltu t0, a3, a2        — node < key?
    ".word 0x11000003\n"             // beqz t0, +3
    ".word 0x00c7302b\n"             // sltu a2, a2, a3        — (delay) key < node?
    ".word 0x10000023\n"             // b cmp2_end
    ".word 0x34060001\n"             // li a2, 1

    // Field 0x18
    ".word 0x50c00003\n"             // beqzl a2, +3
    ".word 0x8c470018\n"             // lw a3, 24(v0)
    ".word 0x1000001f\n"             // b cmp2_end
    ".word 0x34060000\n"             // li a2, 0

    ".word 0x8ca60018\n"             // lw a2, 24(a1)
    ".word 0x00e6402b\n"             // sltu t0, a3, a2
    ".word 0x11000003\n"             // beqz t0, +3
    ".word 0x00c7302b\n"             // sltu a2, a2, a3
    ".word 0x10000019\n"             // b cmp2_end
    ".word 0x34060001\n"             // li a2, 1

    // Field 0x38
    ".word 0x50c00003\n"             // beqzl a2, +3
    ".word 0x8c470038\n"             // lw a3, 56(v0)
    ".word 0x10000015\n"             // b cmp2_end
    ".word 0x34060000\n"             // li a2, 0

    ".word 0x8ca60038\n"             // lw a2, 56(a1)
    ".word 0x00e6402b\n"             // sltu t0, a3, a2
    ".word 0x11000003\n"             // beqz t0, +3
    ".word 0x00c7302b\n"             // sltu a2, a2, a3
    ".word 0x1000000f\n"             // b cmp2_end
    ".word 0x34060001\n"             // li a2, 1

    // Field 0x3C (last)
    ".word 0x50c00003\n"             // beqzl a2, +3
    ".word 0x8c47003c\n"             // lw a3, 60(v0)
    ".word 0x1000000b\n"             // b cmp2_end
    ".word 0x34060000\n"             // li a2, 0

    ".word 0x8ca6003c\n"             // lw a2, 60(a1)
    ".word 0x00e6402b\n"             // sltu t0, a3, a2
    ".word 0x11000003\n"             // beqz t0, +3
    ".word 0x00c7302b\n"             // sltu a2, a2, a3
    ".word 0x10000005\n"             // b cmp2_end
    ".word 0x34060001\n"             // li a2, 1

    ".word 0x10c00003\n"             // beqz a2, cmp2_end
    ".word 0x34060000\n"             // li a2, 0
    ".word 0x10000001\n"             // b cmp2_end
    ".word 0x34060000\n"             // li a2, 0

    // --- cmp2_end: convert to 0/1 ---
    ".word 0x10c00003\n"             // beqz a2, direction     — equal → a2 = 0
    ".word 0x34060000\n"             // li a2, 0
    ".word 0x10000001\n"             // b direction             — node < key → a2 = 1
    ".word 0x34060001\n"             // li a2, 1

    // --- direction: branch on 3-way result in a2 ---
    ".word 0x04c10003\n"             // bgez a2, not_left      — a2 >= 0?
    ".word 0x00000000\n"             // nop
    ".word 0x10000007\n"             // b loop_check           — a2 < 0: go left
    ".word 0x8c420064\n"             // lw v0, 100(v0)         — (delay) v0 = node->mLeft

    ".word 0x18c00003\n"             // blez a2, found         — a2 <= 0 (must be 0)?
    ".word 0x00000000\n"             // nop
    ".word 0x10000003\n"             // b loop_check           — a2 > 0: go right
    ".word 0x8c420068\n"             // lw v0, 104(v0)         — (delay) v0 = node->mRight

    // found: return node
    ".word 0x03e00008\n"             // jr ra
    ".word 0x00000000\n"             // nop

    // loop_check: if node != null, loop back
    ".word 0x5440ff8b\n"             // bnezl v0, loop_top     — if v0 != 0, branch
    ".word 0x8c460010\n"             // lw a2, 16(v0)          — (delay) preload node[0x10]

    // return_null:
    ".word 0x03e00008\n"             // jr ra
    ".word 0x00001025\n"             // move v0, zero          — (delay) return NULL

    ".set reorder\n"
);
