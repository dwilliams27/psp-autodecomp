class cBase;
class cFile;
class cMemPool;
class gcEntity;
class gcPlayer;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

class gcEntity {
public:
    gcPlayer *GetPlayer(void) const;
};

class gcValControllerState : public gcValue {
public:
    gcValControllerState(cBase *);

    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    static int GetEntityControllerId(gcEntity *);
};

extern const char gcValControllerState_base_name[];
extern const char gcValControllerState_base_desc[];

gcValControllerState::gcValControllerState(cBase *parent) {
    ((void **)this)[0] = parent;
    ((void **)this)[1] = (void *)0x388AE8;
    ((int *)this)[2] = 1;
    ((int *)this)[3] = 0;
    ((int *)this)[4] = 0xE;

    char *desired = (char *)this + 0x14;
    gcDesiredObject_ctor(desired, this);
    register void *desiredType asm("a0");
    __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                     : "=r"(desiredType));
    ((void **)this)[6] = desiredType;
    gcDesiredEntityHelper_ctor((char *)this + 0x20, 1, 0, 0);
    ((void **)this)[6] = (void *)0x388A48;
    ((void **)this)[10] = desired;
    ((void **)this)[11] = (void *)0x388568;
    ((char *)this)[0x30] = 1;
    ((char *)this)[0x31] = 0;
    ((int *)this)[13] = 0;
    int desiredOrSelf = (int)desired | 1;
    ((int *)this)[14] = 0;
    register int thisOrSelf asm("a1") = (int)this | 1;
    ((int *)this)[15] = desiredOrSelf;
    ((int *)this)[16] = thisOrSelf;
}

cBase *gcValControllerState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValControllerState *result = 0;
    gcValControllerState *obj =
        (gcValControllerState *)rec->fn(base, 0x44, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValControllerState(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcValControllerState::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.Write(*(const int *)((const char *)this + 0xC));
    wb.Write(*(const int *)((const char *)this + 0x10));

    cTypeMethod *writeMethod =
        (cTypeMethod *)(*(char **)((const char *)this + 0x18) + 0x28);
    char *desiredBase = (char *)this + 0x14;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)writeMethod->fn)(desiredBase + writeMethod->offset,
                                (cFile *)wb._data[0]);

    ((const gcDesiredValue *)((const char *)this + 0x40))->Write(wb);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValControllerState;

const cType *gcValControllerState::GetType(void) const {
    if (!type_gcValControllerState) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValControllerState_base_name,
                                                       gcValControllerState_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValControllerState = cType::InitializeType(
            0, 0, 0x89, type_value, gcValControllerState::New, 0, 0, 0);
    }
    return type_gcValControllerState;
}

int gcValControllerState::GetEntityControllerId(gcEntity *entity) {
    if (entity) {
        gcPlayer *player = entity->GetPlayer();
        int result = -1;
        __asm__ volatile("" : "+r"(result) :: "memory");
        if (player) {
            result = *(int *)((char *)player + 0x20);
        }
        return result;
    }
    return *(int *)0x37D880;
}
