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
    void GetName(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// ── Constructor ──

ePhysicsConstraintConfig::ePhysicsConstraintConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((void **)this)[1] = ePhysicsConstraintConfigclassdesc;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(char *)((char *)this + 0x08) = 0;
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
