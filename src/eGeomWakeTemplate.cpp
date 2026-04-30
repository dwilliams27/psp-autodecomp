// eGeomWakeTemplate methods. In sched=1 eAll_psp engine cluster.

#pragma control sched=1

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cHandle {
public:
    int mHandle;
    void Write(cWriteBlock &) const;
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

class eGeomWake {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class eGeomWakeTemplate : public cObject {
public:
    eGeomWakeTemplate(cBase *);
    ~eGeomWakeTemplate(void);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *target = block + off;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(target, p);
    }
};

extern char eGeomTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_00046C30;

static cType *type_cBase;
static cType *type_eGeom;
static cType *type_eDynamicGeom;
static cType *type_eGeomWake;

struct eGeomWakeTemplateWord {
    int value;
};

// ── eGeomWakeTemplate::eGeomWakeTemplate(cBase *) @ 0x00079d30 ──
#pragma control sched=1
eGeomWakeTemplate::eGeomWakeTemplate(cBase *parent) : cObject(parent) {
    *(volatile float *)((char *)this + 0x44) = 1000.0f;
    *(volatile void **)((char *)this + 4) = (void *)0x00384740;
    *(volatile int *)((char *)this + 0x48) = 10;
    *(volatile int *)((char *)this + 0x4C) = 0;
    *(volatile int *)((char *)this + 0x50) = 20;
    *(volatile int *)((char *)this + 0x54) = 2;
    *(volatile float *)((char *)this + 0x58) = 0.25f;
    *(volatile float *)((char *)this + 0x5C) = 0.5f;
    *(volatile float *)((char *)this + 0x60) = 0.5f;
    *(volatile float *)((char *)this + 0x64) = 12.0f;
    *(volatile float *)((char *)this + 0x68) = 0.1f;
    *(volatile float *)((char *)this + 0x6C) = 0.2f;
    *(volatile int *)((char *)this + 0x70) = -1;
}

// ── eGeomWakeTemplate::GetInstanceType(void) const @ 0x00079ddc ──
#pragma control sched=1
const cType *eGeomWakeTemplate::GetInstanceType(void) const {
    if (type_eGeomWake == 0) {
        if (type_eDynamicGeom == 0) {
            if (type_eGeom == 0) {
                if (type_cBase == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    type_cBase = cType::InitializeType(name, desc, 1,
                                                       0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_cBase,
                                                   0, 0, 0, 0);
            }
            type_eDynamicGeom = cType::InitializeType(0, 0, 0x17, type_eGeom,
                                                      0, 0, 0, 0);
        }
        const cType *parentType = type_eDynamicGeom;
        cBase *(*factory)(cMemPool *, cBase *) = eGeomWake::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eGeomWake = cType::InitializeType(0, 0, 0x257, parentType,
                                               factory, 0, 0, 0);
    }
    return type_eGeomWake;
}

// ── eGeomWakeTemplate::AssignCopy(const cBase *) @ 0x0021207c ──
#pragma control sched=1
void eGeomWakeTemplate::AssignCopy(const cBase *src) {
    eGeomWakeTemplate *other = dcast<eGeomWakeTemplate *>(src);
    cObject::operator=(*other);
    *(float *)((char *)this + 0x44) = *(float *)((char *)other + 0x44);
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 0x48) = *(int *)((char *)other + 0x48);
    eGeomWakeTemplateWord *dst4C = (eGeomWakeTemplateWord *)((char *)this + 0x4C);
    eGeomWakeTemplateWord *src4C = (eGeomWakeTemplateWord *)((char *)other + 0x4C);
    *dst4C = *src4C;
    *(int *)((char *)this + 0x50) = *(int *)((char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(int *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)other + 0x68);
    *(float *)((char *)this + 0x6C) = *(float *)((char *)other + 0x6C);
    __asm__ volatile("" ::: "memory");
    eGeomWakeTemplateWord *dst70 = (eGeomWakeTemplateWord *)((char *)this + 0x70);
    eGeomWakeTemplateWord *src70 = (eGeomWakeTemplateWord *)((char *)other + 0x70);
    *dst70 = *src70;
}

// ── eGeomWakeTemplate::New(cMemPool *, cBase *) static @ 0x0021212c ──
#pragma control sched=1
cBase *eGeomWakeTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomWakeTemplate *result = 0;
    __asm__ volatile("" : "+r"(parent), "+r"(result) :: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eGeomWakeTemplate *obj = (eGeomWakeTemplate *)rec->fn(base, 0x74, 4, 0, 0);
    if (obj != 0) {
        new (obj) eGeomWakeTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eGeomWakeTemplate::GetType(void) const @ 0x002121a8 ──
#pragma control sched=1
const cType *eGeomWakeTemplate::GetType(void) const {
    if (D_00046C30 == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eGeomWakeTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C30 = cType::InitializeType(0, 0, 0x256, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C30;
}

// ── eGeomWakeTemplate::Write(cFile &) const @ 0x00079a9c ──
#pragma control sched=1
void eGeomWakeTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    wb.Write(*(const int *)((const char *)this + 0x48));
    ((const cHandle *)((const char *)this + 0x4C))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x50));
    wb.Write(*(const int *)((const char *)this + 0x54));
    wb.Write(*(const float *)((const char *)this + 0x58));
    wb.Write(*(const float *)((const char *)this + 0x5C));
    wb.Write(*(const float *)((const char *)this + 0x60));
    wb.Write(*(const float *)((const char *)this + 0x64));
    wb.Write(*(const float *)((const char *)this + 0x68));
    wb.Write(*(const float *)((const char *)this + 0x6C));
    wb.Write(*(const unsigned int *)((const char *)this + 0x70));
    wb.End();
}

// ── eGeomWakeTemplate::~eGeomWakeTemplate(void) @ 0x00212340 ──
eGeomWakeTemplate::~eGeomWakeTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

#pragma control sched=2
