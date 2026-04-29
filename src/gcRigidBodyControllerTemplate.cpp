// gcRigidBodyControllerTemplate — methods from gcAll_psp.obj.
// Functions:
//   0x00155da8 gcRigidBodyControllerTemplate::Write(cFile &) const
//   0x0031f280 gcRigidBodyControllerTemplate::New(cMemPool *, cBase *) static
//   0x00155df4 gcRigidBodyControllerTemplate::Read(cFile &, cMemPool *)
//
// The constructor at 0x00155eb0 is matched in src/eStaticSpotLight.cpp.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFile_shim {
public:
    void SetCurrentPos(unsigned int);
};

class gcEntityControllerTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cBaseArray {
public:
    int _count;
    cBase *_owner;
    cBaseArray &operator=(const cBaseArray &);
};

class gcRigidBodyControllerTemplate : public gcEntityControllerTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7B0;

extern "C" {
void *dcastdcast_gcRigidBodyControllerTemplateptr__constcBaseptr(const cBase *);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void gcRigidBodyControllerTemplate__gcRigidBodyControllerTemplate_cBaseptr(
    void *self, cBase *parent);

// ── Write(cFile &) const @ 0x00155da8 ──
void gcRigidBodyControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcEntityControllerTemplate *)this)->Write(file);
    wb.End();
}

// ── New(cMemPool *, cBase *) static @ 0x0031f280 ──
cBase *gcRigidBodyControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcRigidBodyControllerTemplate *result = 0;
    gcRigidBodyControllerTemplate *obj =
        (gcRigidBodyControllerTemplate *)rec->fn(base, 0x50, 0x10, 0, 0);
    if (obj != 0) {
        gcRigidBodyControllerTemplate__gcRigidBodyControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

void cFile_SetCurrentPos(void *, unsigned int);

// ── Read(cFile &, cMemPool *) @ 0x00155df4 ──
int gcRigidBodyControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityControllerTemplate::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// -- AssignCopy(const cBase *) @ 0x0031f1e4 --
typedef int v4sf_t __attribute__((mode(V4SF)));

void gcRigidBodyControllerTemplate::AssignCopy(const cBase *src) {
    gcRigidBodyControllerTemplate *other =
        (gcRigidBodyControllerTemplate *)dcastdcast_gcRigidBodyControllerTemplateptr__constcBaseptr(src);

    const cBaseArray &srcArr0 = *(const cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(srcArr0);

    int i = 0;
    int *dst = (int *)((char *)this + 16);
    int *srcp = (int *)((char *)other + 16);
    do {
        i++;
        *dst = *srcp;
        dst++;
        srcp++;
    } while (i < 2);

    *(int *)((char *)this + 24) = *(const int *)((char *)other + 24);

    ((cBaseArray *)((char *)this + 28))->operator=(*(const cBaseArray *)((char *)other + 28));

    *(v4sf_t *)((char *)this + 0x30) = *(const v4sf_t *)((char *)other + 0x30);

    *(float *)((char *)this + 0x40) = *(const float *)((char *)other + 0x40);
    *(float *)((char *)this + 0x44) = *(const float *)((char *)other + 0x44);
    *(float *)((char *)this + 0x48) = *(const float *)((char *)other + 0x48);
}

// -- GetType(void) const @ 0x0031f2fc --
const cType *gcRigidBodyControllerTemplate::GetType(void) const {
    if (D_0009F7B0 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7B0 = cType::InitializeType(0, 0, 0x142, D_0009A400,
                                           &gcRigidBodyControllerTemplate::New, 0, 0, 0);
    }
    return D_0009F7B0;
}
