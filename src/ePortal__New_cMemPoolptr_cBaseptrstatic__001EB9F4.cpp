// ── ePortal::New(cMemPool *, cBase *) static @ 0x001EB9F4 ──
// Static factory. Mirrors ePortalGroup::New (src/ePortalGroup.cpp) but calls
// the real ePortal::ePortal(cBase*) constructor instead of inlining it.

class cBase;
class cMemPool;

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class ePortal {
public:
    static cBase *New(cMemPool *, cBase *);
};

// real out-of-line constructor: ePortal__ePortal_cBaseptr__00040564
extern "C" void ePortal__ePortal_cBaseptr__00040564(void *, cBase *);

cBase *ePortal::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    cBase *result = 0;
    void *obj = e->fn(base, 0x100, 0x10, 0, 0);
    if (obj != 0) {
        ePortal__ePortal_cBaseptr__00040564(obj, parent);
        result = (cBase *)obj;
    }
    return result;
}
