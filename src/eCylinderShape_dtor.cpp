// eCylinderShape::~eCylinderShape(void) — 0x00074320
//
// Canonical C++ destructor with an optional joint-cleanup virtual call.
// SNC's destructor ABI auto-emits the (this != 0) guard, the chain call to
// ~eShape(), and the deleting-tail dispatch through operator delete (which
// itself does pool/block lookup and calls the slot fn). Same shape as
// eCapsuleShape::~eCapsuleShape (matched in src/eCapsuleShape_dtor.cpp), with
// an added joint cleanup at the top: if (mJoint) joint->vt[0x50](3); mJoint=0.
//
// The branch-likely (beqzl) form falls out of the canonical `if (joint) { }`
// shape because the next-call setup `move a0,this` fits in the not-taken
// delay slot — the compiler recognises that the move is needed only when the
// `if` body is skipped (because the body itself ends with the same move).
//
// Local class redeclarations mirror eCapsuleShape_dtor.cpp / eSphereShape_dtor.cpp.

class cBase;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct JointVtableEntry {
    short adjust;
    short pad;
    void (*fn)(void *self, int kind);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

#pragma control sched=1

class eShape {
public:
    char _pad[0x80];
    eShape(cBase *);
    ~eShape();
};

class eCylinderShape : public eShape {
public:
    float mRadius;       // 0x80
    float mHalfHeight;   // 0x84
    int   mJoint;        // 0x88

    eCylinderShape(cBase *);
    ~eCylinderShape();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eCylinderShapevirtualtable[];

eCylinderShape::~eCylinderShape() {
    *(void **)((char *)this + 4) = eCylinderShapevirtualtable;
    char *joint = (char *)mJoint;
    if (joint != 0) {
        JointVtableEntry *entry = (JointVtableEntry *)(*(char **)(joint + 4) + 0x50);
        entry->fn(joint + entry->adjust, 3);
        mJoint = 0;
    }
}
