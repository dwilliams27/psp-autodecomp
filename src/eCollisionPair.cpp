#include "eCollisionPair.h"

class eRigidBody {
public:
    char _pad[0x98];
    unsigned short mFlags;
};

int eCollisionPair::BodiesSleeping() const {
    int result = 0;
    if (mBodyA->mFlags & 0x40) {
        if (!mBodyB || (mBodyB->mFlags & 0x40)) {
            result = 1;
        }
    }
    return result & 0xFF;
}

int eCollisionPair::AddContact(eCollisionConstraint *c) {
    if ((int)mContactCount < 127) {
        c->mNext = mContactList;
        mContactList = c;
        mContactCount = mContactCount + 1;
        return 1;
    }
    return 0;
}

int eCollisionPair::HasMeshShape() {
    const volatile eRigidBody *a = mBodyA;
    const volatile eRigidBody *b = mBodyB;
    if (((a->mFlags | b->mFlags) & 0x2) && ((a->mFlags | b->mFlags) & 0x400)) {
        return 1;
    }
    return 0;
}

void eCollisionPair::RemoveContacts() {
    while (mContactList) {
        eCollisionConstraint *node = mContactList;
        mContactList = node->mNext;
        eCollisionConstraint::Free(node);
        mContactCount = mContactCount - 1;
    }
}

eCollisionConstraint *eCollisionPair::FindContact(const eContactFeature &f) const {
    eCollisionConstraint *node = mContactList;
    while (node != 0) {
        if (!(node->mFlags & 0x2)) {
            unsigned char match = (node->mFeature.bodyA == f.bodyA && node->mFeature.bodyB == f.bodyB);
            if (match) {
                return node;
            }
        }
        node = node->mNext;
    }
    return 0;
}

void eCollisionPair::CleanupContacts() {
    eCollisionConstraint *node = mContactList;
    eCollisionConstraint *prev = 0;
    while (node != 0) {
        if (node->mFlags & 0x1) {
            if (prev == 0) {
                mContactList = node->mNext;
            } else {
                prev->mNext = node->mNext;
            }
            eCollisionConstraint *toFree = node;
            node = node->mNext;
            eCollisionConstraint::Free(toFree);
            mContactCount = mContactCount - 1;
        } else {
            prev = node;
            node = node->mNext;
        }
    }
}
