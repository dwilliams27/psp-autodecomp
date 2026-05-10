#pragma control sched=1

#include "cBase.h"

class cFile;
class cFileHandle;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

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

extern char eShadowTemplatevirtualtable[];

inline void *operator new(unsigned int, void *p) { return p; }

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

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
    void Read(cReadBlock &);
};

class eMaterial;
class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct WordCopy {
    int value;
};

struct DeleteEntry {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(cObject *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class eShadowTemplate : public cObject {
public:
    eShadowTemplate(cBase *);
    ~eShadowTemplate(void);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C54;

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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oPeShadowTemplatedtv, 0x12c\n");

eShadowTemplate::~eShadowTemplate(void) {
    *(void **)((char *)this + 4) = eShadowTemplatevirtualtable;
    void *field = (char *)this + 0x54;
    if (field != 0) {
        void *entries = *(void **)((char *)this + 0x54);
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
                DeleteEntry *rec = (DeleteEntry *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x54) = 0;
        }
    }
    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x00380C18;
        cObject_dtor((cObject *)this, 0);
    }
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

const cType *eShadowTemplate::GetType(void) const {
    if (D_00046C54 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eShadowTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C54 = cType::InitializeType(0, 0, 0x57, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C54;
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

void eShadowTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    wb.Write(*(const bool *)((const char *)this + 0x48));
    ((const cHandle *)((const char *)this + 0x4C))->Write(wb);
    ((const cHandle *)((const char *)this + 0x50))->Write(wb);

    int *handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->mData;
    int count0 = 0;
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->mData;
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    int *baseHandles = handles;
    int i = 0;
    if (i < count) {
        int offset = 0;
        int *handle = baseHandles + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    wb.Write(*(const float *)((const char *)this + 0x58));
    wb.Write(*(const float *)((const char *)this + 0x5C));
    wb.Write(*(const float *)((const char *)this + 0x60));
    wb.End();
}

int eShadowTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 5, true);
    if ((unsigned int)rb._data[3] != 5) goto fail;
    if (((eDynamicGeomTemplate *)this)->Read(file, pool)) goto success;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    {
        char flag;
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, &flag, 1);
        *(unsigned char *)((char *)this + 0x48) = flag != 0;
    }
    *(int *)((char *)this + 0x4C) = 0;
    {
        cHandle *h = (cHandle *)((char *)this + 0x4C);
        cMemPool *handlePool = cMemPool::GetPoolFromPtr(h);
        h->Read(rb, handlePool);
    }
    *(int *)((char *)this + 0x50) = 0;
    {
        cHandle *h = (cHandle *)((char *)this + 0x50);
        cMemPool *handlePool = cMemPool::GetPoolFromPtr(h);
        h->Read(rb, handlePool);
    }
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->Read(rb);
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x58, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x5C, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x60, 4);
    }
    return result;
}

#pragma control sched=2
