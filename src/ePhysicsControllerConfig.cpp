class cBase;
class cFile;
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
extern "C" void free(void *);

extern char ePhysicsControllerConfigvirtualtable[];
extern const char ePhysicsControllerConfig_base_name[];
extern const char ePhysicsControllerConfig_base_desc[];

class ePhysicsControllerConfig {
public:
    void GetName(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

static cType *type_base;
static cType *type_ePhysicsControllerConfig;

// ── GetName ──
void ePhysicsControllerConfig::GetName(char *buffer) const {
    cStrCopy(buffer, (const char *)this + 8);
}

// ── Write ──
void ePhysicsControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((cName *)((char *)this + 8))->Write(wb);
    wb.End();
}

// ── Read ──
#pragma control sched=1
int ePhysicsControllerConfig::Read(cFile &file, cMemPool *) {
    cReadBlock rb(file, 1, true);
    if (rb._data[3] == 1) {
        ((cName *)((char *)this + 8))->Read(rb);
        return 1;
    }
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

#pragma control sched=2
// ── Destructor ──
struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void ePhysicsControllerConfig___dtor_ePhysicsControllerConfig_void(ePhysicsControllerConfig *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = ePhysicsControllerConfigvirtualtable;
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

// ── GetType ──
#pragma control sched=1
const cType *ePhysicsControllerConfig::GetType(void) const {
    if (!type_ePhysicsControllerConfig) {
        if (!type_base) {
            type_base = cType::InitializeType(ePhysicsControllerConfig_base_name, ePhysicsControllerConfig_base_desc, 1, 0, 0, 0, 0, 0);
        }
        type_ePhysicsControllerConfig = cType::InitializeType(0, 0, 0x23D, type_base, 0, 0, 0, 0);
    }
    return type_ePhysicsControllerConfig;
}
