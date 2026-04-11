#include "eCollisionConstraint.h"

extern eCollisionConstraint *gCollisionConstraintFreeList;  // 0x37D320

eCollisionConstraint *eCollisionConstraint::Allocate(const eContactFeature &feature) {
    eCollisionConstraint *node = gCollisionConstraintFreeList;
    if (node) {
        gCollisionConstraintFreeList = node->mNext;
        __asm__ volatile("" ::: "memory");
        node->mValue = 0.0f;
        eContactFeature *dst = &node->mFeature;
        *dst = feature;
        node->mFlags = 0;
        return node;
    }
    return 0;
}
