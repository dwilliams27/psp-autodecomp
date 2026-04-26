// eShape::~eShape(void) — 0x0002b970
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0)
// guard, the chain-call epilogue (none here — eShape is the root), and
// the deleting-tail dispatch through operator delete. Same pattern as
// src/eSphereShape_dtor.cpp, but with eShape's distinctive null-pool
// fallback to free() in operator delete (the EBOOT bytes show this
// fallback is unique to the base class — derived shape dtors omit it).
//
// Local class redeclaration is used so eShape's static operator delete
// can be inlined into the deleting-tail. include/eShape.h doesn't
// expose operator delete (and shouldn't, since that header is consumed
// by every shape subclass and we don't want their dtors emitting the
// free() fallback that only the base needs).

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eShape {
public:
    char _pad[0x80];
    eShape(cBase *);
    ~eShape();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

eShape::~eShape() {
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
