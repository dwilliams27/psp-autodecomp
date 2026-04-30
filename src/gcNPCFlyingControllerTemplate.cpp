// gcNPCFlyingControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcNPCFlyingControllerTemplate::Write(cFile &) const           @ 0x0015502C  ( 88B)
//   gcNPCFlyingControllerTemplate::AssignCopy(const cBase *)      @ 0x0031D4EC  (164B)
//   gcNPCFlyingControllerTemplate::New(cMemPool *, cBase *)       @ 0x0031D590  (124B)
//
// Sibling pattern of gcNPCBipedControllerTemplate. The NPC variant inherits
// gcFlyingControllerTemplate (same overall layout as gcBipedControllerTemplate
// minus a few trailing fields), and adds one int member at offset +0x50 that
// is serialised by Write() and copied by AssignCopy().

#include "cBase.h"

class cFile;
class cMemPool;
class cType;

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
    cBaseArray &operator=(const cBaseArray &);
};

class gcFlyingControllerTemplate {
public:
    gcFlyingControllerTemplate(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern "C" {
void *dcastdcast_gcNPCFlyingControllerTemplateptr__constcBaseptr(const cBase *);
}

extern char gcNPCFlyingControllerTemplateclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F5A4;
extern cType *D_0009F77C;
extern cType *D_0009F798;

class gcNPCFlyingControllerTemplate : public gcFlyingControllerTemplate {
public:
    gcNPCFlyingControllerTemplate(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// ── gcNPCFlyingControllerTemplate::gcNPCFlyingControllerTemplate(cBase *) @ 0x00155154 ──
gcNPCFlyingControllerTemplate::gcNPCFlyingControllerTemplate(cBase *parent)
    : gcFlyingControllerTemplate(parent) {
    *(char **)((char *)this + 4) = gcNPCFlyingControllerTemplateclassdesc;
    *(int *)((char *)this + 0x50) = 1;
    *(int *)((char *)this + 0x18) |= 1;
}

// ── gcNPCFlyingControllerTemplate::Write(cFile &) const @ 0x0015502C ──
void gcNPCFlyingControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcFlyingControllerTemplate *)this)->Write(file);
    wb.Write(*(const unsigned int *)((char *)this + 0x50));
    wb.End();
}

// ── gcNPCFlyingControllerTemplate::AssignCopy(const cBase *) @ 0x0031D4EC ──
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcNPCFlyingControllerTemplate::AssignCopy(const cBase *src) {
    gcNPCFlyingControllerTemplate *other =
        (gcNPCFlyingControllerTemplate *)dcastdcast_gcNPCFlyingControllerTemplateptr__constcBaseptr(src);

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

    *(int *)((char *)this + 0x50) = *(const int *)((char *)other + 0x50);
}

// ── gcNPCFlyingControllerTemplate::GetType(void) const @ 0x0031D60C ──
const cType *gcNPCFlyingControllerTemplate::GetType(void) const {
    if (D_0009F798 == 0) {
        if (D_0009F77C == 0) {
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
            D_0009F77C = cType::InitializeType(
                0, 0, 0x1AC, D_0009F5A4, 0, 0, 0, 0);
        }
        D_0009F798 = cType::InitializeType(
            0, 0, 0x1AE, D_0009F77C, &gcNPCFlyingControllerTemplate::New, 0, 0, 0);
    }
    return D_0009F798;
}

// ── gcNPCFlyingControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031D590 ──
struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcNPCFlyingControllerTemplate__gcNPCFlyingControllerTemplate_cBaseptr(void *self, cBase *parent);

cBase *gcNPCFlyingControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCFlyingControllerTemplate *result = 0;
    gcNPCFlyingControllerTemplate *obj =
        (gcNPCFlyingControllerTemplate *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCFlyingControllerTemplate__gcNPCFlyingControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
