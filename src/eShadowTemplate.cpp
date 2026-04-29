#pragma control sched=1

#include "cBase.h"
#include "cMemPool.h"
#include "cObject.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char eShadowTemplatevirtualtable[];

inline void *operator new(unsigned int, void *p) { return p; }

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eMaterial;

struct WordCopy {
    int value;
};

class eShadowTemplate : public cObject {
public:
    eShadowTemplate(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

class eShadow {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eDynamicGeom;
static cType *type_eShadowTemplate;

eShadowTemplate::eShadowTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eShadowTemplatevirtualtable;
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 0x48) = 1;
    *(int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x54) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x58) = 0.1f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x5C) = 4.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x60) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x44) = 20.0f;
    __asm__ volatile("" ::: "memory");
}

void eShadowTemplate::AssignCopy(const cBase *base) {
    eShadowTemplate *other = dcast<eShadowTemplate>(base);
    cObject::operator=(*other);
    *(float *)((char *)this + 0x44) =
        *(const float *)((char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 0x48) =
        *(const unsigned char *)((char *)other + 0x48);
    __asm__ volatile("" ::: "memory");
    *(WordCopy *)((char *)this + 0x4C) =
        *(const WordCopy *)((char *)other + 0x4C);
    __asm__ volatile("" ::: "memory");
    *(WordCopy *)((char *)this + 0x50) =
        *(const WordCopy *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x54));
    *(float *)((char *)this + 0x58) =
        *(const float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) =
        *(const float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) =
        *(const float *)((char *)other + 0x60);
}

const cType *eShadowTemplate::GetInstanceType(void) const {
    if (type_eShadowTemplate == 0) {
        if (type_eDynamicGeom == 0) {
            if (type_eGeom == 0) {
                if (type_cBase == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase,
                                                   0, 0, 0, 0);
            }
            type_eDynamicGeom = cType::InitializeType(0, 0, 0x17, type_eGeom,
                                                      0, 0, 0, 0);
        }
        const cType *parentType = type_eDynamicGeom;
        cBase *(*factory)(cMemPool *, cBase *) = eShadow::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eShadowTemplate = cType::InitializeType(0, 0, 0x56, parentType,
                                                     factory, 0, 0, 0);
    }
    return type_eShadowTemplate;
}

cBase *eShadowTemplate::New(cMemPool *pool, cBase *parent) {
    eShadowTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowTemplate *obj = (eShadowTemplate *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        new (obj) eShadowTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
