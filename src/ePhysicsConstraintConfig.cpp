class cBase;
class cFile;
class cMemPool;

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

class cName {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

void cStrCopy(char *, const char *);
void cFile_SetCurrentPos(void *, unsigned int);
void *cMemPool_GetPoolFromPtr(void *);
extern "C" void free(void *);

extern char ePhysicsConstraintConfigclassdesc[];
extern char ePhysicsConstraintConfigvirtualtable[];

class ePhysicsConstraintConfig {
public:
    ePhysicsConstraintConfig(cBase *);
    const cType *GetType(void) const;
    void GetName(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern cType *D_000385DC;
extern cType *D_000469EC;

// ── Constructor ──

ePhysicsConstraintConfig::ePhysicsConstraintConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((void **)this)[1] = ePhysicsConstraintConfigclassdesc;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(char *)((char *)this + 0x08) = 0;
}

// ── GetType ──

const cType *ePhysicsConstraintConfig::GetType(void) const {
    __asm__ volatile("" ::: "memory");
    if (D_000469EC == 0) {
        if (D_000385DC == 0) {
            const char *baseName = (const char *)0x36CD74;
            __asm__ volatile("" : "+r"(baseName) :: "memory");
            const char *baseDesc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(baseDesc) :: "memory");
            D_000385DC = cType::InitializeType(baseName, baseDesc, 1, 0, 0, 0, 0, 0);
        }
        D_000469EC = cType::InitializeType(0, 0, 0x25C, D_000385DC, 0, 0, 0, 0);
    }
    return D_000469EC;
}

// ── GetName ──

void ePhysicsConstraintConfig::GetName(char *buffer) const {
    cStrCopy(buffer, (const char *)this + 8);
}

// ── Write ──

void ePhysicsConstraintConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((cName *)((char *)this + 8))->Write(wb);
    wb.End();
}

// ── Read ──

int ePhysicsConstraintConfig::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((cName *)((char *)this + 8))->Read(rb);
    return result;
}

// ── Destructor ──

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {

void ePhysicsConstraintConfig___dtor_ePhysicsConstraintConfig_void(ePhysicsConstraintConfig *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = ePhysicsConstraintConfigvirtualtable;
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
