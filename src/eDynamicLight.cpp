#include "eDynamicLight.h"

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *, const char *,
                                 unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

extern "C" void eDynamicLight__eDynamicLight_cBaseptr(void *, cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base;
static cType *type_eGeom;
static cType *type_eDynamicGeom;
static cType *type_eDynamicLight;

#pragma control sched=1

int eDynamicLight::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

void eDynamicLight::Draw(const eDrawInfo &) const {
}

cBase *eDynamicLight::New(cMemPool *pool, cBase *parent) {
    eDynamicLight *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eDynamicLight *obj = (eDynamicLight *)rec->fn(base, 0xF0, 0x10, 0, 0);
    if (obj != 0) {
        eDynamicLight__eDynamicLight_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eDynamicLight::GetType(void) const {
    if (!type_eDynamicLight) {
        if (!type_eDynamicGeom) {
            if (!type_eGeom) {
                if (!type_base) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    type_base = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                type_eGeom = cType::InitializeType(0, 0, 0x16, type_base, 0, 0, 0, 0);
            }
            type_eDynamicGeom = cType::InitializeType(0, 0, 0x17, type_eGeom, 0, 0, 0, 0);
        }
        const cType *parentType = type_eDynamicGeom;
        __asm__ volatile("" : "+r"(parentType));
        __asm__ volatile("" ::: "memory");
        cBase *(*factory)(cMemPool *, cBase *) = eDynamicLight::New;
        __asm__ volatile("" : "+r"(factory));
        type_eDynamicLight = cType::InitializeType(0, 0, 0x5A, parentType, factory, 0, 0, 0);
    }
    return type_eDynamicLight;
}
