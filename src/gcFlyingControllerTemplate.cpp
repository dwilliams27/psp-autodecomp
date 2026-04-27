// gcFlyingControllerTemplate.cpp — gcAll_psp.obj
//
// Functions:
//   gcFlyingControllerTemplate::gcFlyingControllerTemplate(cBase *)  @ 0x00153D78  (64B)
//   gcFlyingControllerTemplate::Write(cFile &) const                 @ 0x00153C70  (76B)
//   gcFlyingControllerTemplate::Read(cFile &, cMemPool *)            @ 0x00153CBC  (188B)
//   gcFlyingControllerTemplate::~gcFlyingControllerTemplate(void)    @ 0x0031B730  (204B)
//
// Inherits gcCreatureControllerTemplate.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cBaseArray {
public:
    int mCount;
    cBase *mOwner;
    void RemoveAll(void);
};

class gcCreatureControllerTemplate {
public:
    gcCreatureControllerTemplate(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    char _ccPad[0x40];
};

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void free(void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcFlyingControllerTemplate : public gcCreatureControllerTemplate {
public:
    gcFlyingControllerTemplate(cBase *);
    ~gcFlyingControllerTemplate();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

extern char gcFlyingControllerTemplatevirtualtable[];      // 0x388738
extern char gcCreatureControllerTemplatevirtualtable[];    // 0x37E6A8

// ── ctor (already matched) ──
gcFlyingControllerTemplate::gcFlyingControllerTemplate(cBase *parent)
    : gcCreatureControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcFlyingControllerTemplatevirtualtable;
    *(unsigned int *)((char *)this + 0x18) |= 1;
}

// ── gcFlyingControllerTemplate::Write(cFile &) const ──
void gcFlyingControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcCreatureControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── gcFlyingControllerTemplate::Read(cFile &, cMemPool *) ──
int gcFlyingControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcCreatureControllerTemplate::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── ~gcFlyingControllerTemplate() ──
gcFlyingControllerTemplate::~gcFlyingControllerTemplate() {
    *(char **)((char *)this + 4) = gcFlyingControllerTemplatevirtualtable;
    cBaseArray *a2 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *a1 = (cBaseArray *)((char *)this + 0x8);
    if (a2 != 0) a2->RemoveAll();
    if (a1 != 0) a1->RemoveAll();
    *(char **)((char *)this + 4) = gcCreatureControllerTemplatevirtualtable;
}
