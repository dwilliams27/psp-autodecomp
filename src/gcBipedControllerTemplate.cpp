// gcBipedControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcBipedControllerTemplate::Write(cFile &) const                    @ 0x00140F84  ( 88B)
//   gcBipedControllerTemplate::gcBipedControllerTemplate(cBase *)      @ 0x001410D8  ( 68B)
//   gcBipedControllerTemplate::~gcBipedControllerTemplate(void)        @ 0x002A7964  (204B)
//
// Inheritance chain:
//   cBase -> gcEntityControllerTemplate -> gcCreatureControllerTemplate ->
//   gcBipedControllerTemplate.
//
// The destructor is a deleting destructor (D0): SNC emits the (this != 0) entry
// guard, the inlined chain through every base destructor (with vptr swaps and
// member dtors), and the operator delete dispatch tail.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

extern char gcBipedControllerTemplateclassdesc[];        // 0x38BB60 (live, set in ctor)
extern char gcEntityControllerTemplate_dtor_classdesc[]; // 0x388738 (= gcEntityAttackSetvirtualtable+0x10)
extern char cBase_dtor_classdesc[];                      // 0x37E6A8 (= cFastMemAllocatorvirtualtable+0x50)

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
    void *m_owner;        // +0x00
    void *m_classdesc;    // +0x04
    gcCreatureControllerTemplate(cBase *);
    void Write(cFile &) const;
};

class gcBipedControllerTemplate : public gcCreatureControllerTemplate {
public:
    gcBipedControllerTemplate(cBase *);
    ~gcBipedControllerTemplate();
    void Write(cFile &) const;
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

// ── gcBipedControllerTemplate::gcBipedControllerTemplate(cBase *) @ 0x001410D8 ──
gcBipedControllerTemplate::gcBipedControllerTemplate(cBase *parent)
    : gcCreatureControllerTemplate(parent)
{
    *(char **)((char *)this + 4) = gcBipedControllerTemplateclassdesc;
    *(char *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x18) |= 1;
}

// ── gcBipedControllerTemplate::Write(cFile &) const @ 0x00140F84 ──
void gcBipedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcCreatureControllerTemplate::Write(file);
    wb.Write(*(bool *)((char *)this + 0x50));
    wb.End();
}

// ── gcBipedControllerTemplate::~gcBipedControllerTemplate(void) @ 0x002A7964 ──
//
// SNC inlines the entire base-destructor chain. Both gcCreatureControllerTemplate
// and gcBipedControllerTemplate have empty destructor bodies, so the generated
// code is identical to gcEntityControllerTemplate's destructor: vptr swap to
// gcEntityAttackSetvirtualtable+0x10, two cBaseArray::RemoveAll calls (members
// at +0x1C and +0x8 of the entity-controller layout), then vptr swap to
// cFastMemAllocatorvirtualtable+0x50 (cBase's vtable).
gcBipedControllerTemplate::~gcBipedControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    void *p1 = (char *)this + 0x1C;
    void *p2 = (char *)this + 0x8;
    if (p1) cBaseArray__RemoveAll_void(p1);
    if (p2) cBaseArray__RemoveAll_void(p2);
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}
