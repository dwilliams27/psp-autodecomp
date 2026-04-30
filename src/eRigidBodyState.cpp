class eRigidBodyState;

class eCollisionPair {
public:
    char _pad00[0x10];
    eRigidBodyState *mBodyA;
    char _pad14[0x14];
    eCollisionPair *mNextBodyA;
    char _pad2C[0x04];
    eCollisionPair *mNextBodyB;
};

class eBroadphase {
public:
    void UpdatePairSleepState(eCollisionPair *);
};

class ePhysics {
public:
    static ePhysics *Get(void);

    char _pad00[0x20];
    eBroadphase mBroadphase;
};

class eRigidBodyState {
public:
    char _pad00[0x9C];
    eCollisionPair *mPairs;

    void Update(void);
    void UpdatePairs(void);
    void ReduceVolume(void);
};

void eRigidBodyState::UpdatePairs(void) {
    eCollisionPair *pair = mPairs;
    if (pair != 0) {
        do {
            ePhysics::Get()->mBroadphase.UpdatePairSleepState(pair);
            if (this == pair->mBodyA) {
                pair = pair->mNextBodyA;
            } else {
                pair = pair->mNextBodyB;
            }
        } while (pair != mPairs);
    }
}

void eRigidBodyState::ReduceVolume(void) {
    Update();
}
