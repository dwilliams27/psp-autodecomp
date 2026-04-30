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
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

class gcBipedControllerTemplate {
public:
    gcBipedControllerTemplate(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern "C" {
void *dcastdcast_gcNPCBipedControllerTemplateptr__constcBaseptr(const cBase *);
}

extern char gcNPCBipedControllerTemplateclassdesc[];
extern char gcEntityControllerTemplate_dtor_classdesc[];
extern char cBase_dtor_classdesc[];
extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F5A4;
extern cType *D_0009F5FC;
extern cType *D_0009F790;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcNPCBipedControllerTemplate : public gcBipedControllerTemplate {
public:
    ~gcNPCBipedControllerTemplate();
    gcNPCBipedControllerTemplate(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0ocgcNPCBipedControllerTemplatedtv, 0xb0\n");

// ── gcNPCBipedControllerTemplate::~gcNPCBipedControllerTemplate(void) @ 0x0031cfec ──
gcNPCBipedControllerTemplate::~gcNPCBipedControllerTemplate() {
    *(char **)((char *)this + 4) = gcEntityControllerTemplate_dtor_classdesc;
    cBaseArray *arr1 = (cBaseArray *)((char *)this + 0x1C);
    cBaseArray *arr0 = (cBaseArray *)((char *)this + 0x08);
    if (arr1 != 0) {
        arr1->RemoveAll();
    }
    if (arr0 != 0) {
        arr0->RemoveAll();
    }
    *(char **)((char *)this + 4) = cBase_dtor_classdesc;
}

// ── gcNPCBipedControllerTemplate::gcNPCBipedControllerTemplate(cBase *) @ 0x00154B90 ──
gcNPCBipedControllerTemplate::gcNPCBipedControllerTemplate(cBase *parent)
    : gcBipedControllerTemplate(parent) {
    *(char **)((char *)this + 4) = gcNPCBipedControllerTemplateclassdesc;
    *(int *)((char *)this + 0x60) = 1;
    *(int *)((char *)this + 0x18) |= 1;
}

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

// ── gcNPCBipedControllerTemplate::GetType(void) const @ 0x0031CE98 ──
const cType *gcNPCBipedControllerTemplate::GetType(void) const {
    if (D_0009F790 == 0) {
        if (D_0009F5FC == 0) {
            if (D_0009F5A4 == 0) {
                if (D_0009A400 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_0009A400 = cType::InitializeType(
                        0, 0, 0x9A, D_000385DC, 0, 0, 0, 0);
                }
                D_0009F5A4 = cType::InitializeType(
                    0, 0, 0xB8, D_0009A400, 0, 0, 0, 0);
            }
            D_0009F5FC = cType::InitializeType(
                0, 0, 0xBA, D_0009F5A4, 0, 0, 0, 0);
        }
        D_0009F790 = cType::InitializeType(
            0, 0, 0x100, D_0009F5FC, &gcNPCBipedControllerTemplate::New, 0, 0, 0);
    }
    return D_0009F790;
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
