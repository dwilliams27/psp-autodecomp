typedef int v4sf_t __attribute__((mode(V4SF)));

class eCollisionConstraint {
public:
    eCollisionConstraint *mNext; // 0x00
    float mAccumulatedImpulse;   // 0x04
};

class eCollisionPair {
public:
    char _pad00[0x10];
    void *mBodyA;                         // 0x10
    void *mBodyB;                         // 0x14
    char _pad18[0x08];
    eCollisionPair *mNextActive;          // 0x20
    char _pad24[0x04];
    eCollisionPair *mNextBodyA;           // 0x28
    char _pad2C[0x04];
    eCollisionPair *mNextBodyB;           // 0x30
    char _pad34[0x0C];
    unsigned short mFlags;                // 0x40
    unsigned short mContactCount;         // 0x42
    char _pad44[0x04];
    eCollisionConstraint *mContactList;   // 0x48
    char mAccumulatedImpulses[0x10];      // 0x4C

    void RemoveContacts();
    int BodiesSleeping() const;
};

class eRigidBodyState {
public:
    char _pad00[0x70];
    v4sf_t mAabbMin;              // 0x70
    v4sf_t mAabbMax;              // 0x80
    char _pad90[0x08];
    unsigned short mFlags;        // 0x98
    char _pad9A[0x02];
    eCollisionPair *mPairs;       // 0x9C
    char _padA0[0x18];
    void *mTreeNode;              // 0xB8
};

class eBroadphase {
public:
    char _pad00[0x0C];
    void *mRoot;       // 0x0C
    void *mAllocator;  // 0x10

    void AddRigidBody(eRigidBodyState *body);
    void RemoveRigidBody(eRigidBodyState *body);
    void MarkPairsUnvisited(eRigidBodyState *body);
    void UpdatePairSleepState(eCollisionPair *pair);
    void ClearAccumulatedImpulses(eCollisionPair *pair);
    void UpdatePairs(eRigidBodyState *body);
    void RemovePairs(eRigidBodyState *body);
    void LinkPair(eCollisionPair *pair);
    void UnlinkPair(eCollisionPair *pair);
};

extern "C" {
    void *memset(void *ptr, int value, unsigned int size);
    void *eDynamicAABBTreeNode_eDynamicAABBTreeAllocator_ptreDynamicAABBTreeAllocator_eRigidBodyState___AllocNode_eDynamicAABBTreeNode_eDynamicAABBTreeAllocator_ptr__0020A970(
        void *allocator, void *node);
    void voideDynamicAABBTreeNode_eRigidBodyState___AddObject_eDynamicAABBTreeNodeptr_eDynamicAABBTreeAllocator_eDynamicAABBTreeNode_ptr__0020EAF0(
        void *node, eRigidBodyState *body, void *allocator);
    bool booleDynamicAABBTreeNode_eRigidBodyState___RemoveObject_eDynamicAABBTreeNodeptr_eDynamicAABBTreeAllocator_eDynamicAABBTreeNode_ptr_bool__0020EB90(
        void *node, eRigidBodyState *body, void *allocator, bool doFree);
}

void eBroadphase::MarkPairsUnvisited(eRigidBodyState *body) {
    eCollisionPair *pair = body->mPairs;
    if (pair) {
        do {
            pair->mFlags = pair->mFlags | 1;
            if (body == pair->mBodyA) {
                pair = pair->mNextBodyA;
            } else {
                pair = pair->mNextBodyB;
            }
        } while (pair != body->mPairs);
    }
}

void eBroadphase::UpdatePairSleepState(eCollisionPair *pair) {
    unsigned short flags = pair->mFlags;
    int wasSleeping = (flags & 8) != 0;
    unsigned int isSleeping = pair->BodiesSleeping();
    if (wasSleeping != isSleeping) {
        UnlinkPair(pair);
        LinkPair(pair);
    }
}

void eBroadphase::AddRigidBody(eRigidBodyState *body) {
    eRigidBodyState *arg = body;
    void *root = mRoot;
    void *allocator = mAllocator;
    eBroadphase *self = this;
    if (root == 0) {
        self->mRoot =
            eDynamicAABBTreeNode_eDynamicAABBTreeAllocator_ptreDynamicAABBTreeAllocator_eRigidBodyState___AllocNode_eDynamicAABBTreeNode_eDynamicAABBTreeAllocator_ptr__0020A970(
                allocator, 0);
        allocator = self->mAllocator;
        arg = body;
    }
    __asm__ volatile("" ::: "memory");
    voideDynamicAABBTreeNode_eRigidBodyState___AddObject_eDynamicAABBTreeNodeptr_eDynamicAABBTreeAllocator_eDynamicAABBTreeNode_ptr__0020EAF0(
        self->mRoot, arg, allocator);
    if (!(body->mFlags & 0x20)) {
        self->UpdatePairs(body);
    }
}

void eBroadphase::ClearAccumulatedImpulses(eCollisionPair *pair) {
    if (pair) {
        register eCollisionPair *cur __asm__("s3") = pair;
        register int mask = -3;
        register eCollisionConstraint *contact;
        do {
            contact = cur->mContactList;
            memset(cur->mAccumulatedImpulses, 0, 0x10);
            while (contact) {
                contact->mAccumulatedImpulse = 0.0f;
                contact = contact->mNext;
            }
            cur->RemoveContacts();
            cur->mFlags = cur->mFlags & mask;
            cur = cur->mNextActive;
        } while (cur != pair);
    }
}

void eBroadphase::RemoveRigidBody(eRigidBodyState *body) {
    body->mFlags = body->mFlags & ~1;
    ((volatile eRigidBodyState *)body)->mFlags = body->mFlags;
    void *treeNode = body->mTreeNode;
    booleDynamicAABBTreeNode_eRigidBodyState___RemoveObject_eDynamicAABBTreeNodeptr_eDynamicAABBTreeAllocator_eDynamicAABBTreeNode_ptr_bool__0020EB90(
        treeNode, body, mAllocator, true);
    RemovePairs(body);

    float maxf = 3.40282346638528859812e38f;
    int max = *(int *)&maxf;
    int max2 = *(int *)&maxf;
    int max3 = *(int *)&maxf;
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        :
        : "r"(max), "r"(max2), "r"(max3), "r"(body));
    float minf = -3.40282346638528859812e38f;
    __asm__ volatile("sv.q C120, 0x70(%0)" : : "r"(body));
    int min = *(int *)&minf;
    int min2 = *(int *)&minf;
    int min3 = *(int *)&minf;
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0x80(%3)\n"
        :
        : "r"(min), "r"(min2), "r"(min3), "r"(body));
}
