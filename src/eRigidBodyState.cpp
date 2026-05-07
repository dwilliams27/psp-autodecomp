class eRigidBodyState;
class cBase;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eShape;
class eMeshShape {
public:
    static cBase *New(cMemPool *, cBase *);
};

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
    char _pad00[0x98];
    unsigned short mFlags;
    char _pad9A[2];
    eCollisionPair *mPairs;
    char _padA0[4];
    const eShape *mShape;

    void Update(void);
    void UpdatePairs(void);
    void ReduceVolume(void);
    void SetShape(const eShape *);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046A04;

struct eShapeVtableEntry {
    short offset;
    short _pad;
    const cType *(*fn)(const void *, short, void *);
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

void eRigidBodyState::SetShape(const eShape *shape) {
    mShape = shape;
    if (D_00046A04 == 0) {
        if (D_00040FE4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(
                    (const char *)0x36CD74, (const char *)0x36CD7C,
                    1, 0, 0, 0, 0, 0);
            }
            D_00040FE4 = cType::InitializeType(
                0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
        }
        D_00046A04 = cType::InitializeType(
            0, 0, 0x22C, D_00040FE4, &eMeshShape::New, 0, 0, 0);
    }

    char *vtable = *(char **)((const char *)shape + 4);
    const cType *target = D_00046A04;
    eShapeVtableEntry *entry = (eShapeVtableEntry *)(vtable + 8);
    const cType *type =
        entry->fn((const char *)shape + entry->offset, entry->offset,
                  (void *)entry->fn);
    int same = ((unsigned char)(type == target));
    if (same != 0) {
        unsigned short value =
            *(volatile unsigned short *)((char *)this + 0x98);
        *(volatile unsigned short *)((char *)this + 0x98) = value;
        value = *(volatile unsigned short *)((char *)this + 0x98);
        *(volatile unsigned short *)((char *)this + 0x98) =
            (unsigned short)(value | 0x400);
    }
}
