#include "cBase.h"
#include "cFile.h"

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
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
    void Write(unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cName {
public:
    union {
        int _data[6];
        struct {
            int _pad0[5];
            short mTail0;
            short mTail1;
        };
    };
    void Read(cReadBlock &);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class ePhysicsController;
class eDynamicModel;
class ePhysicsControllerTemplate;

class cBaseArray {
public:
    int mCount;
    cBase *mOwner;
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

extern char ePhysicsControllerTemplateclassdesc[];
extern char ePhysicsControllerTemplate_base_classdesc[];
extern const char ePhysicsControllerTemplate_base_name[];
extern const char ePhysicsControllerTemplate_base_desc[];

static cType *s_type_base;
static cType *s_type_ePhysicsControllerTemplate;

class ePhysicsControllerTemplate {
public:
    cBase *mOwner;             // +0
    void *mClassDesc;          // +4
    cBaseArray mArr1;          // +8
    cBaseArray mArr2;          // +16
    cBaseArray mArr3;          // +24
    unsigned int mField20;     // +32
    unsigned int mField24;     // +36

    ePhysicsControllerTemplate(cBase *b);
    ~ePhysicsControllerTemplate(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static void operator delete(void *);

    static void CreateAndResetInstance(cMemPool *pool, eDynamicModel *model,
                                       const ePhysicsControllerTemplate *tpl,
                                       ePhysicsController **out);
};

// --- ctor ---
ePhysicsControllerTemplate::ePhysicsControllerTemplate(cBase *b) {
    mOwner = b;
    mClassDesc = ePhysicsControllerTemplateclassdesc;
    mArr1.mCount = 0;
    mArr1.mOwner = (cBase *)this;
    mArr2.mCount = 0;
    mArr2.mOwner = (cBase *)this;
    mArr3.mCount = 0;
    mArr3.mOwner = (cBase *)this;
    __asm__ volatile("" ::: "memory");
    mField20 = 1;
    mField24 = 0;
}

// --- Write ---
#pragma control sched=1
void ePhysicsControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    mArr1.Write(wb);
    mArr2.Write(wb);
    mArr3.Write(wb);
    wb.Write(mField20);
    wb.End();
}

#pragma control sched=1
// --- Read ---
int ePhysicsControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 5, true);
    unsigned int version = (unsigned int)rb._data[3];

    if (version >= 6 || version < 1) goto fail;
    if (version != 1) goto read_arr1;

    {
        cName name;
        __asm__ volatile("" ::: "memory");
        name.mTail0 = 0;
        name.mTail1 = 0;
        *(char *)&name = 0;
        name.Read(rb);
        int *dst = (int *)((char *)*(cBase **)mArr1.mCount + 8);
        int n0 = name._data[0];
        int n1 = name._data[1];
        int n2 = name._data[2];
        dst[0] = n0;
        dst[1] = n1;
        dst[2] = n2;
        int n3 = name._data[3];
        int n4 = name._data[4];
        int n5 = name._data[5];
        dst[3] = n3;
        dst[4] = n4;
        dst[5] = n5;
        version = (unsigned int)rb._data[3];
        goto after_arr1;
    }

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

read_arr1:
    mArr1.Read(rb);
    version = (unsigned int)rb._data[3];
after_arr1:
    if (version >= 3) {
        mArr2.Read(rb);
        version = (unsigned int)rb._data[3];
    }

    if (version >= 4) {
        mArr3.Read(rb);
        version = (unsigned int)rb._data[3];
    }

    if (version >= 5) {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mField20, 4);
    }

    return result;
}

// --- GetType ---
#pragma control sched=2
const cType *ePhysicsControllerTemplate::GetType(void) const {
    if (!s_type_ePhysicsControllerTemplate) {
        if (!s_type_base) {
            s_type_base = cType::InitializeType(
                ePhysicsControllerTemplate_base_name,
                ePhysicsControllerTemplate_base_desc,
                1, 0, 0, 0, 0, 0);
        }
        s_type_ePhysicsControllerTemplate = cType::InitializeType(
            0, 0, 0x22F, s_type_base, 0, 0, 0, 0);
    }
    return s_type_ePhysicsControllerTemplate;
}

// --- dtor ---
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

inline void ePhysicsControllerTemplate::operator delete(void *p) {
    void *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    } else {
        free(p);
    }
}

__asm__(".word 0x1000ffff\n.word 0x00000000\n");
__asm__(".size __0oaePhysicsControllerTemplatedtv, 0xe8\n");

#pragma control sched=1
ePhysicsControllerTemplate::~ePhysicsControllerTemplate() {
    mClassDesc = ePhysicsControllerTemplateclassdesc;
    cBaseArray *a3 = &mArr3;
    cBaseArray *a2 = &mArr2;
    cBaseArray *a1 = &mArr1;
    if (a3 != 0) a3->RemoveAll();
    if (a2 != 0) a2->RemoveAll();
    if (a1 != 0) a1->RemoveAll();
    mClassDesc = ePhysicsControllerTemplate_base_classdesc;
}
#pragma control sched=2

// This size override must precede the method definition; SNC's emitted function
// body still places the two words after the normal epilogue.
__asm__(".word 0x1000ffff\n.word 0x00000000\n");
__asm__(".size __0faePhysicsControllerTemplateWCreateAndResetInstanceP6IcMemPoolP6NeDynamicModelPC6aePhysicsControllerTemplatePP6SePhysicsControllerT, 0x120\n");

// --- CreateAndResetInstance ---
#pragma control sched=1
struct VtEntry {
    short adj;
    short _pad;
    void *fn;
};

void ePhysicsControllerTemplate::CreateAndResetInstance(
    cMemPool *pool, eDynamicModel *model,
    const ePhysicsControllerTemplate *tpl,
    ePhysicsController **out)
{
    if (tpl == 0) {
        if (*out != 0) {
            ePhysicsController *existing = *out;
            void *vt = *(void **)((char *)existing + 4);
            register VtEntry *e __asm__("$5") = (VtEntry *)((char *)vt + 0x50);
            __asm__ volatile("" : "+r"(e));
            register short adj __asm__("$6") = e->adj;
            void *adjusted = (char *)existing + adj;
            register void (*fn)(void *, int) __asm__("$6") = (void (*)(void *, int))e->fn;
            fn(adjusted, 3);
            *out = 0;
        }
        return;
    }

    if (*out == 0 || *(const ePhysicsControllerTemplate **)((char *)(*out) + 8) != tpl) {
        if (*out != 0) {
            ePhysicsController *existing = *out;
            void *vt = *(void **)((char *)existing + 4);
            register VtEntry *e __asm__("$5") = (VtEntry *)((char *)vt + 0x50);
            __asm__ volatile("" : "+r"(e));
            register short adj __asm__("$6") = e->adj;
            void *adjusted = (char *)existing + adj;
            register void (*fn)(void *, int) __asm__("$6") = (void (*)(void *, int))e->fn;
            fn(adjusted, 3);
            *out = 0;
        }
        void *tvt = *(void **)((char *)tpl + 4);
        register VtEntry *te __asm__("$5") = (VtEntry *)((char *)tvt + 0x78);
        __asm__ volatile("" : "+r"(te));
        register short typeAdj __asm__("$4") = te->adj;
        void *typeAdjusted = (char *)tpl + typeAdj;
        register void *(*typeFn)(void *) __asm__("$5") = (void *(*)(void *))te->fn;
        void *typeObj = typeFn(typeAdjusted);
        void *(*creator)(cMemPool *, eDynamicModel *) =
            *(void *(**)(cMemPool *, eDynamicModel *))((char *)typeObj + 0x10);
        ePhysicsController *created = (ePhysicsController *)creator(pool, model);
        *out = created;
        *(const ePhysicsControllerTemplate **)((char *)created + 8) = tpl;
        *(eDynamicModel **)((char *)(*out) + 0xC) = model;
    }

    if (*out != 0) {
        ePhysicsController *existing = *out;
        void *vt = *(void **)((char *)existing + 4);
        register VtEntry *e __asm__("$5") = (VtEntry *)((char *)vt + 0x38);
        __asm__ volatile("" : "+r"(e));
        register short adj __asm__("$6") = e->adj;
        void *adjusted = (char *)existing + adj;
        register void (*fn)(void *, cMemPool *, int) __asm__("$7") =
            (void (*)(void *, cMemPool *, int))e->fn;
        fn(adjusted, pool, 1);
    }
}
