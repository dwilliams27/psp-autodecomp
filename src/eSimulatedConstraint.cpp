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

extern cType *D_000385DC;
extern cType *D_00046BC4;

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

#pragma control sched=1
void eSimulatedConstraint::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

int eSimulatedConstraint::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
#pragma control sched=2

#pragma control sched=1
const cType *eSimulatedConstraint::GetType(void) const {
    if (D_00046BC4 == 0) {
        if (D_000385DC == 0) {
            const char *name = (const char *)0x36CD74;
            const char *desc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        D_00046BC4 = cType::InitializeType(0, 0, 0x25E, D_000385DC, 0, 0, 0, 0);
    }
    return D_00046BC4;
}
#pragma control sched=2

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
