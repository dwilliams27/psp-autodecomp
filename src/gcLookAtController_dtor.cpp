// gcLookAtController::~gcLookAtController(void) — 0x00147104
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcPartialEntityController(), and the deleting-tail
// dispatch through operator delete (which itself does pool/block lookup and
// calls the slot fn). Same shape as eSphereShape::~eSphereShape (matched in
// src/eSphereShape_dtor.cpp).
//
// Local class redeclarations are used so gcLookAtController can declare
// inheritance from gcPartialEntityController without clashing with the flat
// layout used by the data-access methods in src/gcLookAtController.cpp.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcPartialEntityController {
public:
    char _pad[0x40];
    gcPartialEntityController(cBase *);
    ~gcPartialEntityController();
};

class gcLookAtController : public gcPartialEntityController {
public:
    gcLookAtController(cBase *);
    ~gcLookAtController();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char gcLookAtControllervirtualtable[];

gcLookAtController::~gcLookAtController() {
    *(void **)((char *)this + 4) = gcLookAtControllervirtualtable;
}
