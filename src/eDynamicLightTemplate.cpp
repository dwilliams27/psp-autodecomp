// eDynamicLightTemplate.cpp - decompiled from eAll_psp.obj
//
// Functions:
//   0x0007744c  eDynamicLightTemplate::Write(cFile &) const
//   0x00077608  eDynamicLightTemplate::eDynamicLightTemplate(cBase *)
//   0x0020fe38  eDynamicLightTemplate::AssignCopy(const cBase *)
//   0x0020fe94  eDynamicLightTemplate::New(cMemPool *, cBase *) static

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cReadBlock;
class cType;

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

template <class T>
class cArray {
public:
    void Read(cReadBlock &);
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eDynamicLightTemplate {
public:
    eDynamicLightTemplate(cBase *);
    ~eDynamicLightTemplate(void);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            __asm__ volatile("" : "+r"(p));
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
};

class eDynamicLight {
public:
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
extern cType *D_00040FF4;
extern cType *D_000469A8;
extern cType *D_000469C0;
extern cType *D_000469E0;
extern cType *D_00046C14;
extern cType *D_00046C18;

extern "C" {
    void eDynamicLightTemplate__eDynamicLightTemplate_cBaseptr(void *self, cBase *parent);
}
extern "C" void cObject_ctor(cObject *, cBase *) asm("__0oHcObjectctP6FcBase");
extern "C" void cObject_dtor(cObject *, int) asm("__0oHcObjectdtv");

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

    int count0 = 0;
    int *handles = *(int **)((char *)this + 0x48);
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = *(int **)((char *)this + 0x48);
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }
    int *handleBase = handles;

    int i = 0;
    int offset = 0;
    if (i < count) {
        int *handle = handleBase + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.End();
}
#pragma control sched=2

// -- eDynamicLightTemplate::Read(cFile &, cMemPool *) @ 0x0007752C --
#pragma control sched=1
int eDynamicLightTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eDynamicGeomTemplate *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((cArray<cHandleT<eMaterial> > *)((char *)this + 0x48))->Read(rb);
    void *fileObj = *(void **)&rb._data[0];
    void *handle = *(void **)fileObj;
    __asm__ volatile("" : "+r"(handle));
    cFileSystem::Read(handle, (char *)this + 0x4C, 4);
    return result;
}
#pragma control sched=2

// -- eDynamicLightTemplate::GetInstanceType(void) const @ 0x00077658 --
#pragma control sched=1
const cType *eDynamicLightTemplate::GetInstanceType(void) const {
    if (D_00046C18 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicLight::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C18 = cType::InitializeType(0, 0, 0x5A, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C18;
}
#pragma control sched=2

// -- eDynamicLightTemplate::eDynamicLightTemplate(cBase *) @ 0x00077608 --
#pragma control sched=1
eDynamicLightTemplate::eDynamicLightTemplate(cBase *parent) {
    cObject_ctor((cObject *)this, parent);
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

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oVeDynamicLightTemplatedtv, 0x12c\n");

// -- eDynamicLightTemplate::~eDynamicLightTemplate(void) @ 0x002100a8 --
#pragma control sched=1
eDynamicLightTemplate::~eDynamicLightTemplate(void) {
    *(void **)((char *)this + 4) = (void *)0x00384278;
    void *field = (char *)this + 0x48;
    if (field != 0) {
        void *entries = *(void **)((char *)this + 0x48);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                __asm__ volatile("" : "+r"(basePtr));
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x48) = 0;
        }
    }
    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x00380C18;
        cObject_dtor((cObject *)this, 0);
    }
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
