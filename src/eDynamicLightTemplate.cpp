// eDynamicLightTemplate.cpp - decompiled from eAll_psp.obj
//
// Functions:
//   0x0007744c  eDynamicLightTemplate::Write(cFile &) const
//   0x00077608  eDynamicLightTemplate::eDynamicLightTemplate(cBase *)
//   0x0020fe38  eDynamicLightTemplate::AssignCopy(const cBase *)
//   0x0020fe94  eDynamicLightTemplate::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
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

class eMaterial;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eDynamicLightTemplate : public cObject {
public:
    eDynamicLightTemplate(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C14;

extern "C" {
    void eDynamicLightTemplate__eDynamicLightTemplate_cBaseptr(void *self, cBase *parent);
}

// -- eDynamicLightTemplate::AssignCopy(const cBase *) @ 0x0020fe38 --
#pragma control sched=1
void eDynamicLightTemplate::AssignCopy(const cBase *base) {
    eDynamicLightTemplate *other = dcast<eDynamicLightTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->operator=(
        *(const cArrayBase<cHandleT<eMaterial> > *)((char *)other + 0x48));
    *(float *)((char *)this + 0x4C) = *(const float *)((char *)other + 0x4C);
}
#pragma control sched=2

// -- eDynamicLightTemplate::Write(cFile &) const @ 0x0007744c --
#pragma control sched=1
void eDynamicLightTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eDynamicGeomTemplate *)this)->Write(file);

    cHandle *handles0 = *(cHandle **)((const char *)this + 0x48);
    int count0 = 0;
    if (handles0 != 0) {
        count0 = *((int *)handles0 - 1) & 0x3FFFFFFF;
    }
    wb.Write(count0);

    cHandle *handles = *(cHandle **)((const char *)this + 0x48);
    int count = 0;
    if (handles != 0) {
        count = *((int *)handles - 1) & 0x3FFFFFFF;
    }
    int offset = 0;
    int i = 0;
    if (i < count) {
        cHandle *handle = handles + offset;
        do {
            handle->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.End();
}
#pragma control sched=2

// -- eDynamicLightTemplate::eDynamicLightTemplate(cBase *) @ 0x00077608 --
#pragma control sched=1
eDynamicLightTemplate::eDynamicLightTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x00384278;
    *(void **)((char *)this + 0x48) = 0;
    *(float *)((char *)this + 0x4C) = 1.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x44) = 50.0f;
}
#pragma control sched=2

// -- eDynamicLightTemplate::New(cMemPool *, cBase *) static @ 0x0020fe94 --
#pragma control sched=1
cBase *eDynamicLightTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicLightTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicLightTemplate *obj = (eDynamicLightTemplate *)rec->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        eDynamicLightTemplate__eDynamicLightTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── eDynamicLightTemplate::GetType(void) const @ 0x0020FF10 ──
#pragma control sched=1
const cType *eDynamicLightTemplate::GetType(void) const {
    if (D_00046C14 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicLightTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C14 = cType::InitializeType(0, 0, 0x5B, parentType,
                                           factory, 0, 0, 0);
    }
    return D_00046C14;
}
#pragma control sched=2
