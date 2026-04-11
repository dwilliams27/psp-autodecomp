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
