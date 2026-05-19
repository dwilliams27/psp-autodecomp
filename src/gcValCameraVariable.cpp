#include "cBase.h"

class cMemPool;
class cFile;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcLValue {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);
    void *Get(bool) const;
};

class gcDesiredEntity {
public:
    char _pad[0x2C];
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValCameraVariable {
public:
    char _pad0[0x10];
    int mField10;
    gcDesiredEntity mDesiredEntity;
    unsigned char mField40;
    char _pad41[3];
    int mField44;

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Set(float);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    ~gcValCameraVariable(void);
    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];
extern char gcValCameraVariablevirtualtable[];
void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredCamera___dtor_gcDesiredCamera_void(void *, int);

struct WriteRecord {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct gcValCameraVariable_Camera {
    char pad000[0x20];
    int frame;
    char pad024[0xE9D];
    unsigned char viewId;
};

struct gcValCameraVariable_Frame {
    char pad00[0x70];
    float f70;
    float f74;
    float f78;
    char pad7C[0x1C];
    float f98;
};

struct gcValCameraVariable_Block {
    char pad00[0x08];
    unsigned char b08;
    unsigned char b09;
    unsigned char b0A;
    char pad0B[0x05];
    float f10;
    char pad14[0x28];
    float f3C;
    float f40;
    float f44;
    char pad48[0x14];
    float f5C;
    char pad60[0x10];
    float f70;
    float f74;
    char pad78[0x18];
    float f90;
    float f94;
    float f98;
    char pad9C[0x7C];
    float f118;
    char pad11C[0x14];
    float f130;
    float f134;
    char pad138[0x18];
    float f150;
    float f154;
    float f158;
    char pad15C[0x34];
    float f190;
    float f194;
};

// 0x003223ac, 88B
void gcValCameraVariable::AssignCopy(const cBase *base) {
    gcValCameraVariable *other = dcast<gcValCameraVariable>(base);
    mField10 = other->mField10;
    mDesiredEntity.operator=(other->mDesiredEntity);
    mField40 = other->mField40;
    mField44 = other->mField44;
}

// 0x00322404, 160B
cBase *gcValCameraVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *rec = (AllocEntry *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValCameraVariable *result = 0;
    gcValCameraVariable *obj =
        (gcValCameraVariable *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)obj = parent;
        *(void **)((char *)obj + 4) = gcValCameraVariablevirtualtable;
        gcDesiredCamera_gcDesiredCamera((char *)obj + 8, (cBase *)obj);
        obj->mField40 = 0;
        obj->mField44 = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x003225f8, 132B
void gcValCameraVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcLValue *)this)->Write(file);
    char *typeInfo = *(char **)((const char *)this + 12);
    WriteRecord *rec = (WriteRecord *)(typeInfo + 0x28);
    char *base = (char *)this + 8;
    rec->fn(base + rec->offset, wb.file);
    wb.Write(mField40);
    wb.Write(mField44);
    wb.End();
}

// 0x00322b50, 424B
void gcValCameraVariable::Set(float value) {
    gcValCameraVariable_Camera *camera =
        (gcValCameraVariable_Camera *)((gcDesiredCamera *)((char *)this + 8))->Get(true);
    if (camera == 0) {
        return;
    }

    if (camera->viewId != mField40) {
        return;
    }
    __asm__ volatile("" ::: "memory");

    int frame = camera->frame;
    int selector = mField44;
    char *base = (char *)camera;
    base += frame * 0x240;
    base -= 0x210;
    __asm__ volatile("" : "+r"(base));
    selector -= 3;
    if ((unsigned int)selector >= 30) {
        return;
    }

    gcValCameraVariable_Frame *frameData = (gcValCameraVariable_Frame *)base;
    gcValCameraVariable_Block *block =
        (gcValCameraVariable_Block *)(base + 0xA0);
    switch (selector) {
    case 0:
        frameData->f70 = value;
        break;
    case 1:
        frameData->f74 = value;
        break;
    case 2:
        frameData->f78 = value;
        break;
    case 3:
        block->f190 = value;
        break;
    case 4:
        block->f194 = value;
        break;
    case 5: {
        float zero = 0.0f;
        int flag = 0;
        if (value != zero) {
            flag = 1;
        }
        block->b08 = flag;
        break;
    }
    case 6:
        block->f70 = value;
        break;
    case 7:
        block->f74 = value;
        break;
    case 8:
        block->f10 = value;
        break;
    case 9:
        block->f3C = value;
        break;
    case 10:
        block->f44 = value;
        break;
    case 11:
        frameData->f98 = value;
        break;
    case 12:
        block->f98 = value;
        break;
    case 13:
        block->f118 = value;
        break;
    case 14:
        block->f5C = value;
        break;
    case 15: {
        float zero = 0.0f;
        int flag = 0;
        if (value != zero) {
            flag = 1;
        }
        block->b09 = flag;
        break;
    }
    case 16: {
        float zero = 0.0f;
        int flag = 0;
        if (value != zero) {
            flag = 1;
        }
        block->b0A = flag;
        break;
    }
    case 17:
        block->f90 = value;
        break;
    case 18:
        block->f94 = value;
        break;
    case 19:
        block->f40 = value;
        break;
    case 20:
        block->f130 = value;
        break;
    case 21:
        block->f134 = value;
        break;
    case 22:
        block->f158 = value;
        break;
    case 23:
        block->f150 = value;
        break;
    case 24:
        block->f154 = value;
        break;
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        break;
    }
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValCameraVariable;

// 0x003224a4, 340B
const cType *gcValCameraVariable::GetType(void) const {
    if (!type_gcValCameraVariable) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValCameraVariable = cType::InitializeType(
            0, 0, 0x253, type_variable, gcValCameraVariable::New, 0, 0, 0);
    }
    return type_gcValCameraVariable;
}

// 0x00322f2c, 136B
gcValCameraVariable::~gcValCameraVariable(void) {
    *(void **)((char *)this + 4) = gcValCameraVariablevirtualtable;
    gcDesiredCamera___dtor_gcDesiredCamera_void((char *)this + 8, 2);
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
