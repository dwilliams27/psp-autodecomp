// gcBipedController — gcAll_psp.obj.
//
// Functions:
//   gcBipedController::Write(cFile &) const          @ 0x0014111c  (76B)
//   gcBipedController::Read(cFile &, cMemPool *)     @ 0x00141168 (188B)
//   gcBipedController::OnDeactivated(void)           @ 0x00142154  (48B)
//   gcBipedController::~gcBipedController(void)      @ 0x002a7b84 (124B)
//
// gcBipedController inherits from gcCreatureController, which inherits
// from gcEntityController.

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
    void OnDeactivated(void);
    void SetPhysicsController(const cType *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcBipedController : public gcCreatureController {
public:
    gcBipedController(cBase *);
    ~gcBipedController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void OnDeactivated(void);
    const cType *GetType(void) const;
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

extern char gcBipedControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F5A8;
extern cType *D_0009F600;

// ── gcBipedController::gcBipedController(cBase *) @ 0x00141224 ──
gcBipedController::gcBipedController(cBase *parent)
    : gcCreatureController(parent) {
    void *vtable = gcBipedControllervirtualtable;
    __asm__ volatile("mtc1 $0, $f12" ::: "$f12", "memory");
    *(void **)((char *)this + 4) = vtable;
    __asm__ volatile(
        "mfc1 $5, $f12\n"
        "mfc1 $6, $f12\n"
        "sh $0, 0x90(%0)\n"
        "mfc1 $4, $f12\n"
        "mtv $5, S120\n"
        "mtv $4, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0xA0(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "memory");
    *(int *)((char *)this + 0xB0) = 0;
    __asm__ volatile(
        "lui $4, 0x3f80\n"
        "swc1 $f12, 0xB4(%0)\n"
        "mtc1 $4, $f12\n"
        :
        : "r"(this)
        : "$4", "$f12", "memory");
    __asm__ volatile(
        "swc1 $f12, 0xB8(%0)\n"
        :
        : "r"(this)
        : "$f12", "memory");
}

// ── gcBipedController::Write(cFile &) const @ 0x0014111c ──
void gcBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcCreatureController *)this)->Write(file);
    wb.End();
}

// ── gcBipedController::Read(cFile &, cMemPool *) @ 0x00141168 ──
int gcBipedController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcCreatureController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcBipedController::OnDeactivated(void) @ 0x00142154 ──
void gcBipedController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}

// ── gcBipedController::GetType(void) const @ 0x002a7a30 ──
const cType *gcBipedController::GetType(void) const {
    if (D_0009F600 == 0) {
        if (D_0009F5A8 == 0) {
            if (D_0009A404 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                }
                D_0009A404 = cType::InitializeType(
                    0, 0, 0x99, D_000385E0, 0, 0, 0, 0);
            }
            D_0009F5A8 = cType::InitializeType(
                0, 0, 0xB9, D_0009A404, 0, 0, 0, 0);
        }
        D_0009F600 = cType::InitializeType(
            0, 0, 0xBB, D_0009F5A8, 0, 0, 0, 0);
    }
    return D_0009F600;
}

// ── gcBipedController::~gcBipedController(void) @ 0x002a7b84 ──
gcBipedController::~gcBipedController() {
    *(void **)((char *)this + 4) = gcBipedControllervirtualtable;
}
