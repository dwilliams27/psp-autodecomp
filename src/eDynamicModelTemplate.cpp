#include "cObject.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cName;
class cType;
class eSkin;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
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

template <class T>
class cHandleT {
public:
    int mHandle;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cBaseArray {
public:
    void *mData;
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mHandle;
    void Write(cWriteBlock &) const;
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct EDMWord {
    int value;
};

struct DynamicModelTemplateWriteFields {
    char pad50[0x50];
    float field50;
    float field54;
    float field58;
    int field5C;
    bool field60;
    bool field61;
};

extern char eDynamicModelvirtualtable[];
extern char eDynamicModelTemplatevirtualtable[];

class eDynamicModelTemplate : public cObject {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_000469FC;

void eDynamicModelTemplate::AssignCopy(const cBase *base) {
    eDynamicModelTemplate *other = dcast<eDynamicModelTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 0x44) = *(const float *)((const char *)other + 0x44);
    *(EDMWord *)((char *)this + 0x48) =
        *(const EDMWord *)((const char *)other + 0x48);
    ((cArrayBase<cHandleT<eSkin> > *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<cHandleT<eSkin> > *)((const char *)other + 0x4C));
    *(float *)((char *)this + 0x50) = *(const float *)((const char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((const char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((const char *)other + 0x58);
    *(int *)((char *)this + 0x5C) = *(const int *)((const char *)other + 0x5C);
    *(volatile unsigned char *)((char *)this + 0x60) = *(const volatile unsigned char *)((const char *)other + 0x60);
    *(volatile unsigned char *)((char *)this + 0x61) = *(const volatile unsigned char *)((const char *)other + 0x61);
    ((cBaseArray *)((char *)this + 0x64))->operator=(
        *(const cBaseArray *)((const char *)other + 0x64));
    ((cBaseArray *)((char *)this + 0x6C))->operator=(
        *(const cBaseArray *)((const char *)other + 0x6C));
    *(volatile int *)((char *)this + 0x74) = *(const volatile int *)((const char *)other + 0x74);
    ((cArrayBase<cName> *)((char *)this + 0x78))->operator=(
        *(const cArrayBase<cName> *)((const char *)other + 0x78));
    ((cArrayBase<short> *)((char *)this + 0x7C))->operator=(
        *(const cArrayBase<short> *)((const char *)other + 0x7C));
}

cBase *eDynamicModelTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicModelTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eDynamicModelTemplate *obj =
        (eDynamicModelTemplate *)entry->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        new ((cObject *)obj) cObject(parent);
        *(void **)((char *)obj + 4) = eDynamicModelvirtualtable;
        *(float *)((char *)obj + 0x44) = 1000.0f;
        __asm__ volatile("" ::: "memory");
        *(void **)((char *)obj + 4) = eDynamicModelTemplatevirtualtable;
        *(int *)((char *)obj + 0x48) = 0;
        *(int *)((char *)obj + 0x4C) = 0;
        *(float *)((char *)obj + 0x50) = 1000.0f;
        __asm__ volatile("" ::: "memory");
        *(float *)((char *)obj + 0x54) = 0.0f;
        *(float *)((char *)obj + 0x58) = 3.0f;
        __asm__ volatile("" ::: "memory");
        *(int *)((char *)obj + 0x5C) = 0;
        *(unsigned char *)((char *)obj + 0x60) = 0;
        *(unsigned char *)((char *)obj + 0x61) = 0;
        *(int *)((char *)obj + 0x64) = 0;
        *(eDynamicModelTemplate **)((char *)obj + 0x68) = obj;
        *(int *)((char *)obj + 0x6C) = 0;
        *(eDynamicModelTemplate **)((char *)obj + 0x70) = obj;
        *(int *)((char *)obj + 0x74) = 0;
        *(int *)((char *)obj + 0x78) = 0;
        *(int *)((char *)obj + 0x7C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1
const cType *eDynamicModelTemplate::GetType(void) const {
    if (D_000469FC == 0) {
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
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicModelTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_000469FC = cType::InitializeType(0, 0, 0x2C, parentType, factory,
                                           0, 0, 0);
    }
    return D_000469FC;
}
#pragma control sched=2
void eDynamicModelTemplate::Write(cFile &file) const {
    const eDynamicModelTemplate *self = this;
    const DynamicModelTemplateWriteFields *fields =
        (const DynamicModelTemplateWriteFields *)self;
    cFile &out = file;
    cWriteBlock wb(out, 0x10);
    ((const eDynamicGeomTemplate *)self)->Write(out);
    ((const cHandle *)((const char *)self + 0x48))->Write(wb);

    {
        int *temp_a0 = *(int **)((char *)self + 0x4C);
        int var_s1 = 0;
        if (temp_a0 != 0) {
            var_s1 = temp_a0[-1] & 0x3FFFFFFF;
        }
        wb.Write(var_s1);
    }

    {
        int *temp_a0 = *(int **)((char *)self + 0x4C);
        int var_s1 = 0;
        if (temp_a0 != 0) {
            var_s1 = temp_a0[-1] & 0x3FFFFFFF;
        }
        int var_s2 = 0;
        if (var_s1 > 0) {
            char *var_s3 = (char *)temp_a0;
            do {
                ((const cHandle *)var_s3)->Write(wb);
                var_s2 += 1;
                var_s3 = (char *)var_s3 + 4;
            } while (var_s2 < var_s1);
        }
    }

    wb.Write(fields->field60);
    wb.Write(fields->field50);
    wb.Write(fields->field5C);
    ((const cBaseArray *)((const char *)self + 0x64))->Write(wb);

    char *temp_a0_3 = *(char **)((char *)self + 0x78);
    int var_s3_2 = 0;
    if (temp_a0_3 != 0) {
        var_s3_2 = ((int *)temp_a0_3)[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_s3_2);

    char *temp_s3 = *(char **)((char *)self + 0x78);
    int var_s1_3 = 0;
    if (temp_s3 != 0) {
        var_s1_3 = ((int *)temp_s3)[-1] & 0x3FFFFFFF;
    }

    int var_s2_2 = 0;
    if (var_s1_3 > 0) {
        char *var_s3_3 = temp_s3;
        do {
            ((const cName *)var_s3_3)->Write(wb);
            var_s2_2 += 1;
            var_s3_3 = (char *)var_s3_3 + 0x18;
        } while (var_s2_2 < var_s1_3);
    }

    wb.Write(fields->field61);
    wb.Write(fields->field54);
    ((const cBaseArray *)((const char *)self + 0x6C))->Write(wb);
    wb.Write(fields->field58);
    wb.WriteBase(*(cBase *const *)((const char *)self + 0x74));
    wb.End();
}
#pragma control sched=1
