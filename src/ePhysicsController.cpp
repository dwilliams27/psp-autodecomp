#include "cBase.h"
#include "cFile.h"

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
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
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

struct mVec3;

class ePhysicsController {
public:
    cBase *mOwner;
    void *mClassDesc;
    int mField8;
    int mFieldC;

    ePhysicsController(cBase *);
    ~ePhysicsController(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;

    void ApplyUnembedImpulse(int, const mVec3 &);
    void SnapConstraints(void);
    void OnSnappedTo(void);
    int GetCollisionMask(void) const;
    void SetCollisionMask(unsigned int);

    static void operator delete(void *p);
};

class eSimulatedMotor {
public:
    ~eSimulatedMotor(void);
    static void operator delete(void *p);
};

extern char ePhysicsControllerclassdesc[];
extern const char ePhysicsController_base_name[];
extern const char ePhysicsController_base_desc[];

static cType *type_base;
static cType *type_ePhysicsController;

// --- ctor ---
ePhysicsController::ePhysicsController(cBase *b) {
    mOwner = b;
    mClassDesc = ePhysicsControllerclassdesc;
    mField8 = 0;
    mFieldC = 0;
}

// --- Write ---
void ePhysicsController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

void cFile_SetCurrentPos(void *, unsigned int);

// --- Read ---
int ePhysicsController::Read(cFile &file, cMemPool *) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    unsigned int version = (unsigned int)rb._data[3];
    if (version == 1u)
        goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// --- dtor ---
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

inline void ePhysicsController::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

ePhysicsController::~ePhysicsController(void) {
    mClassDesc = ePhysicsControllerclassdesc;
}

#pragma control sched=1

inline void eSimulatedMotor::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        char *base = block + rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    } else {
        free(p);
    }
}

eSimulatedMotor::~eSimulatedMotor(void) {
    *(void **)((char *)this + 4) = ePhysicsControllerclassdesc;
}

#pragma control sched=2

// --- trivial virtual stubs ---
void ePhysicsController::ApplyUnembedImpulse(int, const mVec3 &) {
}

void ePhysicsController::SnapConstraints(void) {
}

void ePhysicsController::OnSnappedTo(void) {
}

int ePhysicsController::GetCollisionMask(void) const {
    return 0;
}

void ePhysicsController::SetCollisionMask(unsigned int) {
}

// --- GetType ---
const cType *ePhysicsController::GetType(void) const {
    if (!type_ePhysicsController) {
        if (!type_base) {
            type_base = cType::InitializeType(ePhysicsController_base_name, ePhysicsController_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_ePhysicsController = cType::InitializeType(0, 0, 0x232, type_base, 0, 0, 0, 0);
    }
    return type_ePhysicsController;
}
