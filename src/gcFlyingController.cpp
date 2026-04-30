// gcFlyingController — methods from gcAll_psp.obj.
//
// gcFlyingController inherits directly from gcEntityController (confirmed
// by the destructor's chain call at 0x00110958). Write/Read forward to
// gcCreatureController via reinterpret-style C-cast (static dispatch);
// gcCreatureController itself extends gcEntityController so the layouts
// share their gcEntityController base prefix.

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F5A8;
extern cType *D_0009F780;

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

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
    void Update(cTimeValue);
    void OnSelected();
    void OnDeselected();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcFlyingController : public gcCreatureController {
public:
    gcFlyingController(cBase *);
    ~gcFlyingController();
    const cType *GetType(void) const;
    void Update(cTimeValue);
    void OnSelected();
    void OnDeselected();
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

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern char gcFlyingControllervirtualtable[];

// ── gcFlyingController::gcFlyingController(cBase *) @ 0x00153ec0 ──
gcFlyingController::gcFlyingController(cBase *parent)
    : gcCreatureController(parent) {
    *(void **)((char *)this + 4) = gcFlyingControllervirtualtable;
    *(short *)((char *)this + 0x90) = 0;
    *(short *)((char *)this + 0x92) = 0;
    __vec_new((char *)this + 0x94, 1, 6, (void (*)(void *))0x24400C);
}

// ── gcFlyingController::GetType(void) const @ 0x0031b7fc ──
const cType *gcFlyingController::GetType(void) const {
    if (D_0009F780 == 0) {
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
        D_0009F780 = cType::InitializeType(
            0, 0, 0x1AD, D_0009F5A8, 0, 0, 0, 0);
    }
    return D_0009F780;
}

// ── Update (0x00153f14) ──

void gcFlyingController::Update(cTimeValue t) {
    gcEntityController::Update(t);
}

// ── OnSelected (0x00153f30) ──

void gcFlyingController::OnSelected() {
    gcEntityController::OnSelected();
}

// ── OnDeselected (0x00153f4c) ──

void gcFlyingController::OnDeselected() {
    gcEntityController::OnDeselected();
}

// ── Write(cFile &) const @ 0x00153db8 ──
void gcFlyingController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcCreatureController::Write(file);
    wb.End();
}

// ── Read(cFile &, cMemPool *) @ 0x00153e04 ──
int gcFlyingController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcCreatureController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── ~gcFlyingController(void) @ 0x0031b950 ──
gcFlyingController::~gcFlyingController() {
    *(void **)((char *)this + 4) = gcFlyingControllervirtualtable;
}
