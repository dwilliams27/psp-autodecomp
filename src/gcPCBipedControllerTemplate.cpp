// gcPCBipedControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcPCBipedControllerTemplate::Write(cFile &) const          @ 0x00152480  (76B)
//   gcPCBipedControllerTemplate::AssignCopy(const cBase *)     @ 0x00319fdc (164B)
//   gcPCBipedControllerTemplate::New(cMemPool *, cBase *)      @ 0x0031a080 (124B)

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
void *dcastdcast_gcPCBipedControllerTemplateptr__constcBaseptr(const cBase *);
}

extern char gcPCBipedControllerTemplateclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F5A4;
extern cType *D_0009F5FC;
extern cType *D_0009F76C;

class gcPCBipedControllerTemplate : public gcBipedControllerTemplate {
public:
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// ── gcPCBipedControllerTemplate::Write(cFile &) const @ 0x00152480 ──
void gcPCBipedControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── gcPCBipedControllerTemplate::AssignCopy(const cBase *) @ 0x00319fdc ──
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcPCBipedControllerTemplate::AssignCopy(const cBase *src) {
    gcPCBipedControllerTemplate *other =
        (gcPCBipedControllerTemplate *)dcastdcast_gcPCBipedControllerTemplateptr__constcBaseptr(src);

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
}

// ── gcPCBipedControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031a080 ──
struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcPCBipedControllerTemplate__gcPCBipedControllerTemplate_cBaseptr(void *self, cBase *parent);

cBase *gcPCBipedControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPCBipedControllerTemplate *result = 0;
    gcPCBipedControllerTemplate *obj =
        (gcPCBipedControllerTemplate *)rec->fn(base, 0x60, 0x10, 0, 0);
    if (obj != 0) {
        gcPCBipedControllerTemplate__gcPCBipedControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcPCBipedControllerTemplate::GetType(void) const @ 0x0031a0fc ──
const cType *gcPCBipedControllerTemplate::GetType(void) const {
    if (D_0009F76C == 0) {
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
        D_0009F76C = cType::InitializeType(
            0, 0, 0xBC, D_0009F5FC, &gcPCBipedControllerTemplate::New, 0, 0, 0);
    }
    return D_0009F76C;
}
