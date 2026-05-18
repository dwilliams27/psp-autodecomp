// src/gcBipedControllerTemplate_dtor.cpp - split TU for destructor only.
// ODR-WARNING: local redeclaration of gcBipedControllerTemplate (and its
// bases) with only the destructor + helper operator delete. The other
// methods of this class live in src/gcBipedControllerTemplate.cpp.
//
// The trailing `b $; nop` trap (2 instructions) that SNC emits after the
// jr-ra epilogue for this dtor is appended via the file-scope `.word` /
// `.size` pattern (same trick as src/eBspTree_dtor.cpp).

class cBase;
class cMemPool;

extern char gcEntityControllerTemplate_dtor_classdesc[]; // 0x388738
extern char cBase_dtor_classdesc[];                      // 0x37E6A8

extern "C" {
    void cBaseArray__RemoveAll_void(void *);
    void *cMemPool__GetPoolFromPtr_constvoidptrstatic(const void *);
    void free(void *);
}

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcCreatureControllerTemplate {
public:
    void *m_owner;
    void *m_classdesc;
};

class gcBipedControllerTemplate : public gcCreatureControllerTemplate {
public:
    ~gcBipedControllerTemplate();
};

inline void operator delete(void *p) {
    void *pool = cMemPool__GetPoolFromPtr_constvoidptrstatic(p);
    if (pool != 0) {
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    } else {
        free(p);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oZgcBipedControllerTemplatedtv, 0xCC\n");

gcBipedControllerTemplate::~gcBipedControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    void *p1 = (char *)this + 0x1C;
    void *p2 = (char *)this + 0x8;
    if (p1) cBaseArray__RemoveAll_void(p1);
    if (p2) cBaseArray__RemoveAll_void(p2);
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}
