// gcNPCBipedControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcNPCBipedControllerTemplate::Write(cFile &) const          @ 0x00154A68  ( 88B)
//   gcNPCBipedControllerTemplate::AssignCopy(const cBase *)     @ 0x0031CD70  (172B)
//   gcNPCBipedControllerTemplate::New(cMemPool *, cBase *)      @ 0x0031CE1C  (124B)
//
// Sibling-style structure to gcPCBipedControllerTemplate (same base class
// gcBipedControllerTemplate). The NPC variant has one extra int member at
// offset +0x60 that is serialised by Write() and copied by AssignCopy().

#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    int _count;
    cBase *_owner;
    cBaseArray &operator=(const cBaseArray &);
};

class gcBipedControllerTemplate {
public:
    void Write(cFile &) const;
};

extern "C" {
void *dcastdcast_gcNPCBipedControllerTemplateptr__constcBaseptr(const cBase *);
}

class gcNPCBipedControllerTemplate : public gcBipedControllerTemplate {
public:
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// ── gcNPCBipedControllerTemplate::Write(cFile &) const @ 0x00154A68 ──
void gcNPCBipedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcBipedControllerTemplate *)this)->Write(file);
    wb.Write(*(const unsigned int *)((char *)this + 0x60));
    wb.End();
}

// ── gcNPCBipedControllerTemplate::AssignCopy(const cBase *) @ 0x0031CD70 ──
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcNPCBipedControllerTemplate::AssignCopy(const cBase *src) {
    gcNPCBipedControllerTemplate *other =
        (gcNPCBipedControllerTemplate *)dcastdcast_gcNPCBipedControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);

    *(unsigned char *)((char *)this + 0x50) = *(const unsigned char *)((char *)other + 0x50);

    *(int *)((char *)this + 0x60) = *(const int *)((char *)other + 0x60);
}

// ── gcNPCBipedControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031CE1C ──
struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcNPCBipedControllerTemplate__gcNPCBipedControllerTemplate_cBaseptr(void *self, cBase *parent);

cBase *gcNPCBipedControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCBipedControllerTemplate *result = 0;
    gcNPCBipedControllerTemplate *obj =
        (gcNPCBipedControllerTemplate *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCBipedControllerTemplate__gcNPCBipedControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
