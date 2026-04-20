#include "cBase.h"
#include "cFile.h"

class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
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

class ePhysicsController {
public:
    cBase *mOwner;
    void *mClassDesc;
    int mField8;
    int mFieldC;

    ePhysicsController(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
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

extern "C" void ePhysicsController___dtor_ePhysicsController_void(ePhysicsController *self, int flags) {
    if (self != 0) {
        self->mClassDesc = ePhysicsControllerclassdesc;
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
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
