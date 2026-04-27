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

class gcRigidBodyControllerTemplate : public gcEntityControllerTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

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
