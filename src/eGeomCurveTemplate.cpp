// eGeomCurveTemplate.cpp - decompiled from eAll_psp.obj
// Parent codegen matches the eGeomTemplate/cObject destructor and copy chain.

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

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
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

class eGeomCurve {
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eGeomCurveTemplate : public cObject {
public:
    float mField44;          // 0x44
    cHandle mHandle48;       // 0x48
    int mField4C;            // 0x4C
    float mField50;          // 0x50
    bool mField54;           // 0x54
    char _pad55[3];
    float mField58;          // 0x58
    float mField5C;          // 0x5C
    float mField60;          // 0x60
    float mField64;          // 0x64
    float mField68;          // 0x68
    int mField6C;            // 0x6C
    bool mField70;           // 0x70
    char _pad71[3];
    float mField74;          // 0x74
    unsigned int mField78;   // 0x78

    eGeomCurveTemplate(cBase *);
    ~eGeomCurveTemplate(void);
    void AssignCopy(const cBase *);
    const cType *GetInstanceType(void) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
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
extern cType *D_00040FF4;
extern cType *D_000469A8;
extern cType *D_000469C0;
extern cType *D_000469E0;
extern cType *D_00046C1C;
extern cType *D_00046C20;

struct eGeomCurveTemplateWord {
    int value;
};

// eGeomCurveTemplate::eGeomCurveTemplate(cBase *) @ 0x00077f48
#pragma control sched=1
eGeomCurveTemplate::eGeomCurveTemplate(cBase *parent) : cObject(parent) {
    *(volatile float *)((char *)this + 0x44) = 1000.0f;
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
    *(volatile int *)((char *)this + 0x48) = 0;
    *(volatile int *)((char *)this + 0x4C) = 10;
    *(volatile float *)((char *)this + 0x50) = 0.25f;
    *(volatile unsigned char *)((char *)this + 0x54) = 1;
    *(volatile float *)((char *)this + 0x58) = 1.0f;
    *(volatile float *)((char *)this + 0x5C) = 1.0f;
    *(volatile float *)((char *)this + 0x60) = 1.0f;
    *(volatile float *)((char *)this + 0x64) = 0.0f;
    *(volatile float *)((char *)this + 0x68) = 0.0f;
    *(volatile int *)((char *)this + 0x6C) = 0;
    *(volatile unsigned char *)((char *)this + 0x70) = 0;
    *(volatile float *)((char *)this + 0x74) = 0.0f;
    unsigned int one = 1;
    __asm__ volatile("" : "+r"(one));
    *(volatile unsigned int *)((char *)this + 0x78) = one;
}
#pragma control sched=2

// eGeomCurveTemplate::GetInstanceType(void) const @ 0x00077fdc
#pragma control sched=1
const cType *eGeomCurveTemplate::GetInstanceType(void) const {
    if (D_00046C20 == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(
                        name, desc, 1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(
                    0, 0, 0x16, D_000385DC, 0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(
                0, 0, 0x17, D_00040FF4, 0, 0, 0, 0);
        }
        const cType *parentType = D_000469C0;
        cBase *(*factory)(cMemPool *, cBase *) = &eGeomCurve::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C20 = cType::InitializeType(0, 0, 0x1D4, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C20;
}
#pragma control sched=2

// eGeomCurveTemplate::AssignCopy(const cBase *) @ 0x00210814
#pragma control sched=1
void eGeomCurveTemplate::AssignCopy(const cBase *base) {
    eGeomCurveTemplate *src = dcast<eGeomCurveTemplate *>(base);
    cObject::operator=(*src);
    *(float *)((char *)this + 0x44) = *(float *)((char *)src + 0x44);
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplateWord *dst48 = (eGeomCurveTemplateWord *)((char *)this + 0x48);
    eGeomCurveTemplateWord *src48 = (eGeomCurveTemplateWord *)((char *)src + 0x48);
    *dst48 = *src48;
    *(int *)((char *)this + 0x4C) = *(int *)((char *)src + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)src + 0x50);
    *(unsigned char *)((char *)this + 0x54) = *(unsigned char *)((char *)src + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)src + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)src + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)src + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)src + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)src + 0x68);
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplateWord *dst6c = (eGeomCurveTemplateWord *)((char *)this + 0x6C);
    eGeomCurveTemplateWord *src6c = (eGeomCurveTemplateWord *)((char *)src + 0x6C);
    *dst6c = *src6c;
    *(unsigned char *)((char *)this + 0x70) = *(unsigned char *)((char *)src + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)src + 0x74);
    *(int *)((char *)this + 0x78) = *(int *)((char *)src + 0x78);
}
#pragma control sched=2

// eGeomCurveTemplate::New(cMemPool *, cBase *) static @ 0x002108d4
#pragma control sched=1
cBase *eGeomCurveTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomCurveTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomCurveTemplate *obj = (eGeomCurveTemplate *)rec->fn(base, 0x7C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eGeomCurveTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// eGeomCurveTemplate::GetType(void) const @ 0x00210950
#pragma control sched=1
const cType *eGeomCurveTemplate::GetType(void) const {
    if (D_00046C1C == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eGeomCurveTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C1C = cType::InitializeType(0, 0, 0x1D3, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C1C;
}
#pragma control sched=2

// eGeomCurveTemplate::Write(cFile &) const @ 0x00077c34
#pragma control sched=1
void eGeomCurveTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    mHandle48.Write(wb);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField54);
    wb.Write(mField5C);
    wb.Write(mField60);
    wb.Write(mField64);
    wb.Write(mField68);
    wb.Write(mField6C);
    wb.Write(mField58);
    wb.Write(mField70);
    wb.Write(mField74);
    wb.Write(mField78);
    wb.End();
}
#pragma control sched=2

// eGeomCurveTemplate::~eGeomCurveTemplate(void) @ 0x00210ae8
#pragma control sched=1
eGeomCurveTemplate::~eGeomCurveTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}
#pragma control sched=2
