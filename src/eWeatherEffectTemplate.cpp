class cBase;
class cFile;
class cMemPool;
class cType;

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
    void Write(cWriteBlock &) const;
};

class eGeomTemplate {
public:
    void Write(cFile &) const;
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
extern cType *D_00046BA0;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
};

class eWeatherEffectTemplate : public cObject {
public:
    eWeatherEffectTemplate(cBase *);
    ~eWeatherEffectTemplate(void);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eGeomTemplatevirtualtable[];

extern "C" {
    void eWeatherEffectTemplate__eWeatherEffectTemplate_cBaseptr(void *self, cBase *parent);
}

template <class T> T *dcast(const cBase *);

#pragma control sched=2

// eWeatherEffectTemplate::eWeatherEffectTemplate(cBase *) @ 0x00061384
eWeatherEffectTemplate::eWeatherEffectTemplate(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
    *(volatile int *)((char *)this + 0x44) = 0x1F4;
    *(volatile float *)((char *)this + 0x48) = 0.0f;
    *(volatile float *)((char *)this + 0x4C) = -90.0f;
    *(volatile float *)((char *)this + 0x50) = 1.0f;
    *(volatile float *)((char *)this + 0x54) = 0.1f;
    *(volatile float *)((char *)this + 0x58) = 0.0f;
    *(volatile float *)((char *)this + 0x5C) = 0.0f;
    *(volatile float *)((char *)this + 0x60) = 50.0f;
    *(volatile float *)((char *)this + 0x64) = 10.0f;
    *(volatile float *)((char *)this + 0x68) = 1.0f;
    *(volatile float *)((char *)this + 0x6C) = 0.25f;
    *(volatile int *)((char *)this + 0x70) = 0;
    *(volatile unsigned char *)((char *)this + 0x74) = 0;
    *(volatile float *)((char *)this + 0x78) = 10.0f;
    *(volatile float *)((char *)this + 0x7C) = 2.5f;
    *(volatile float *)((char *)this + 0x80) = 10.0f;
    *(volatile float *)((char *)this + 0x84) = 100.0f;
}

#pragma control sched=1

// eWeatherEffectTemplate::~eWeatherEffectTemplate(void) @ 0x00061448
eWeatherEffectTemplate::~eWeatherEffectTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

// eWeatherEffectTemplate::AssignCopy(const cBase *) @ 0x002078d0
struct eWeatherEffectTemplate_block_4 { int _[1]; };

void eWeatherEffectTemplate::AssignCopy(const cBase *src) {
    eWeatherEffectTemplate *other = dcast<eWeatherEffectTemplate>(src);
    cObject::operator=(*other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(float *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(float *)((char *)other + 0x68);
    *(float *)((char *)this + 0x6C) = *(float *)((char *)other + 0x6C);
    __asm__ volatile("" ::: "memory");
    eWeatherEffectTemplate_block_4 *dst70 = (eWeatherEffectTemplate_block_4 *)((char *)this + 0x70);
    eWeatherEffectTemplate_block_4 *src70 = (eWeatherEffectTemplate_block_4 *)((char *)other + 0x70);
    *dst70 = *src70;
    *(unsigned char *)((char *)this + 0x74) = *(unsigned char *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    *(float *)((char *)this + 0x7C) = *(float *)((char *)other + 0x7C);
    *(float *)((char *)this + 0x80) = *(float *)((char *)other + 0x80);
    *(float *)((char *)this + 0x84) = *(float *)((char *)other + 0x84);
}

// eWeatherEffectTemplate::New(cMemPool *, cBase *) static @ 0x002079a0
cBase *eWeatherEffectTemplate::New(cMemPool *pool, cBase *parent) {
    eWeatherEffectTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eWeatherEffectTemplate *obj = (eWeatherEffectTemplate *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        eWeatherEffectTemplate__eWeatherEffectTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eWeatherEffectTemplate::GetType(void) const {
    if (D_00046BA0 == 0) {
        if (D_000469A8 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *kindName = (const char *)0x36CE2C;
            const char *kindDesc = (const char *)0x36CE3C;
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_000469A8 = cType::InitializeType(0, 0, 0x20, parentType,
                                               0, kindName, kindDesc, 5);
        }
        const cType *parentType = D_000469A8;
        cBase *(*factory)(cMemPool *, cBase *) = &eWeatherEffectTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BA0 = cType::InitializeType(0, 0, 0x191, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046BA0;
}

#pragma control sched=2

#pragma control sched=1
void eWeatherEffectTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const eGeomTemplate *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x70))->Write(wb);
    wb.Write(*(int *)((const char *)this + 0x44));
    wb.Write(*(float *)((const char *)this + 0x60));
    wb.Write(*(float *)((const char *)this + 0x48));
    wb.Write(*(float *)((const char *)this + 0x4C));
    wb.Write(*(float *)((const char *)this + 0x50));
    wb.Write(*(float *)((const char *)this + 0x58));
    wb.Write(*(float *)((const char *)this + 0x64));
    wb.Write(*(float *)((const char *)this + 0x5C));
    wb.Write(*(float *)((const char *)this + 0x54));
    wb.Write(*(float *)((const char *)this + 0x68));
    wb.Write(*(float *)((const char *)this + 0x6C));
    wb.Write(*(const bool *)((const char *)this + 0x74));
    wb.Write(*(float *)((const char *)this + 0x78));
    wb.Write(*(float *)((const char *)this + 0x7C));
    wb.Write(*(float *)((const char *)this + 0x80));
    wb.Write(*(float *)((const char *)this + 0x84));
    wb.End();
}
#pragma control sched=2
