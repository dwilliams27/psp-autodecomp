#include "cObject.h"

class cBase;
class cFile;
class cMemPool;
class cType;
class eMaterial;

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
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
    void Write(bool);
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

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

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C48;
extern cType *D_00046C4C;

class eProjector {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eProjectorTemplate : public cObject {
public:
    eProjectorTemplate(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// -- eProjectorTemplate::eProjectorTemplate(cBase *) @ 0x0007d17c --
#pragma control sched=1
eProjectorTemplate::eProjectorTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x00384B20;
    *(void **)((char *)this + 0x48) = 0;
    *(unsigned char *)((char *)this + 0x4C) = 0;
    *(float *)((char *)this + 0x50) = 45.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x54) = 0.1f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x58) = 4.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x5C) = 1.0f;
    *(float *)((char *)this + 0x60) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x44) = 20.0f;
    __asm__ volatile("" ::: "memory");
}
#pragma control sched=2

// -- eProjectorTemplate::AssignCopy(const cBase *) @ 0x00214e18 --
#pragma control sched=1
void eProjectorTemplate::AssignCopy(const cBase *base) {
    eProjectorTemplate *other = dcast<eProjectorTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((const char *)other + 0x48));
    *(unsigned char *)((char *)this + 0x4C) = *(const unsigned char *)((const char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(const float *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((const char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((const char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(const float *)((const char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(const float *)((const char *)other + 0x60);
}
#pragma control sched=2

// -- eProjectorTemplate::New(cMemPool *, cBase *) static @ 0x00214e9c --
#pragma control sched=1
cBase *eProjectorTemplate::New(cMemPool *pool, cBase *parent) {
    eProjectorTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eProjectorTemplate *obj = (eProjectorTemplate *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        new (obj) eProjectorTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// -- eProjectorTemplate::GetInstanceType(void) const @ 0x0007d1fc --
#pragma control sched=1
const cType *eProjectorTemplate::GetInstanceType(void) const {
    if (D_00046C4C == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        cBase *(*factory)(cMemPool *, cBase *) = &eProjector::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C4C = cType::InitializeType(0, 0, 0x58, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C4C;
}
#pragma control sched=2

// -- eProjectorTemplate::GetType(void) const @ 0x00214f18 --
#pragma control sched=1
const cType *eProjectorTemplate::GetType(void) const {
    if (D_00046C48 == 0) {
        if (D_000469E0 == 0) {
            if (D_000469A8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CE2C;
                const char *kindDesc = (const char *)0x36CE3C;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, parentType, 0, kindName, kindDesc, 5);
            }
            D_000469E0 = cType::InitializeType(0, 0, 0x22, D_000469A8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469E0;
        cBase *(*factory)(cMemPool *, cBase *) = &eProjectorTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C48 = cType::InitializeType(0, 0, 0x59, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C48;
}
#pragma control sched=2

// -- eProjectorTemplate::Write(cFile &) const @ 0x0007cf30 --
#pragma control sched=1
void eProjectorTemplate::Write(cFile &file) const {
    cHandle *temp_s2;
    int var_a0;
    int var_s1;
    int var_s2;
    int var_s3;

    cWriteBlock wb(file, 1);
    ((const eDynamicGeomTemplate *)this)->Write(file);

    temp_s2 = *(cHandle **)((char *)this + 0x48);
    var_a0 = 0;
    if (temp_s2 != 0) {
        var_a0 = ((int *)temp_s2)[-1];
        var_a0 &= 0x3FFFFFFF;
    }
    wb.Write(var_a0);

    temp_s2 = *(cHandle **)((char *)this + 0x48);
    var_s1 = 0;
    if (temp_s2 != 0) {
        var_s1 = ((int *)temp_s2)[-1];
        var_s1 &= 0x3FFFFFFF;
    }
    cHandle *temp_a0 = temp_s2;
    for (var_s2 = 0, var_s3 = 0; var_s2 < var_s1; var_s2++, var_s3 += 4) {
        ((cHandle *)((char *)temp_a0 + var_s3))->Write(wb);
    }

    wb.Write(*(const bool *)((const char *)this + 0x4C));
    wb.Write(*(const float *)((const char *)this + 0x50));
    wb.Write(2, (const float *)((const char *)this + 0x5C));
    wb.Write(*(const float *)((const char *)this + 0x54));
    wb.Write(*(const float *)((const char *)this + 0x58));
    wb.End();
}
#pragma control sched=2
