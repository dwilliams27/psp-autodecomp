// gcCreatureController — gcAll_psp.obj.
//
// Functions:
//   gcCreatureController::Write(cFile &) const         @ 0x0013ca68  (76B)
//   gcCreatureController::Read(cFile &, cMemPool *)    @ 0x0013cab4 (188B)
//   gcCreatureController::~gcCreatureController(void)  @ 0x0029429c (124B)
//
// gcCreatureController inherits from gcEntityController.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

void cFile_SetCurrentPos(void *, unsigned int);

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        char *base = block + off;
        fn(base, p);
    }
};

extern char gcCreatureControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F5A8;

// ── gcCreatureController::Write(cFile &) const @ 0x0013ca68 ──
void gcCreatureController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcEntityController *)this)->Write(file);
    wb.End();
}

// ── gcCreatureController::Read(cFile &, cMemPool *) @ 0x0013cab4 ──
int gcCreatureController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcCreatureController::GetType(void) const @ 0x00294184 ──
const cType *gcCreatureController::GetType(void) const {
    if (D_0009F5A8 == 0) {
        if (D_0009A404 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_0009A404 = cType::InitializeType(0, 0, 0x99, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F5A8 = cType::InitializeType(0, 0, 0xB9, D_0009A404,
                                           0, 0, 0, 0);
    }
    return D_0009F5A8;
}

// ── gcCreatureController::~gcCreatureController(void) @ 0x0029429c ──
gcCreatureController::~gcCreatureController() {
    *(void **)((char *)this + 4) = gcCreatureControllervirtualtable;
}
