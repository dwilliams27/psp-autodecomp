class cBase;
class cFile;
class cMemPool;

extern char eSimulatedConstraintclassdesc[];
extern char eSimulatedConstraintvirtualtable[];

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

void cFile_SetCurrentPos(void *, unsigned int);
void *cMemPool_GetPoolFromPtr(void *);
extern "C" void free(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern const char eSimulatedConstraint_base_name[];
extern const char eSimulatedConstraint_base_desc[];

static cType *type_eSimulatedConstraint_base;
static cType *type_eSimulatedConstraint;

class eSimulatedConstraint {
public:
    eSimulatedConstraint(cBase *parent);
    cBase *base;
    void *vtable;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

eSimulatedConstraint::eSimulatedConstraint(cBase *parent) {
    base = parent;
    vtable = eSimulatedConstraintclassdesc;
}

void eSimulatedConstraint::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

int eSimulatedConstraint::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

const cType *eSimulatedConstraint::GetType(void) const {
    if (!type_eSimulatedConstraint) {
        if (!type_eSimulatedConstraint_base) {
            type_eSimulatedConstraint_base = cType::InitializeType(eSimulatedConstraint_base_name, eSimulatedConstraint_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_eSimulatedConstraint = cType::InitializeType(0, 0, 0x25E, type_eSimulatedConstraint_base, 0, 0, 0, 0);
    }
    return type_eSimulatedConstraint;
}

extern "C" {

void eSimulatedConstraint___dtor_eSimulatedConstraint_void(eSimulatedConstraint *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eSimulatedConstraintvirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
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

}

class eSimulatedMotor {
public:
    void Write(cFile &) const;
};

void eSimulatedMotor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}
