// eSphereShape::~eSphereShape(void) — 0x00067c60
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~eShape(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot
// fn). Same shape as eSoundData::~eSoundData (matched in src/eSoundData.cpp).
//
// Local class redeclarations are used so eSphereShape can declare inheritance
// from eShape with a tighter base size than include/eShape.h's 0x80 padding
// (which would push mRadius past offset 0x74). Sizes here only matter for
// member access; this file does not touch mRadius.

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
    char _pad[0x74];
    eShape(cBase *);
    ~eShape();
};

class eSphereShape : public eShape {
public:
    float mRadius;  // 0x74 from start, immediately after eShape

    eSphereShape(cBase *);
    ~eSphereShape();
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

extern char eSphereShapevirtualtable[];

eSphereShape::~eSphereShape() {
    *(void **)((char *)this + 4) = eSphereShapevirtualtable;
}
