// eCapsuleShape::~eCapsuleShape(void) — 0x0006a11c
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~eShape(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot
// fn). Same shape as eSphereShape::~eSphereShape (matched in src/eSphereShape_dtor.cpp).
//
// Local class redeclarations are used so eCapsuleShape can declare inheritance
// from eShape with a tighter base size than include/eShape.h's 0x80 padding.
// Sizes here matter only for layout; this file does not touch radius/halfHeight.

class cBase;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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

class eCapsuleShape : public eShape {
public:
    float radius;       // 0x80
    float halfHeight;   // 0x84

    eCapsuleShape(cBase *);
    ~eCapsuleShape();
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

extern char eCapsuleShapevirtualtable[];

eCapsuleShape::~eCapsuleShape() {
    *(void **)((char *)this + 4) = eCapsuleShapevirtualtable;
}
