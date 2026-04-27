// Multi-class wrapper file for small leaf-of-leaf methods.
// Functions: gcRigidBodyController::OnDeactivated,
//            gcRigidBodyController::Write(cFile &) const,
//            gcRigidBodyController::~gcRigidBodyController(void),
//            gcRigidBodyController::New(cMemPool *, cBase *) static,
//            cLanguageSubscriber::Attach,
//            gcState::GetName(char *) const,
//            gcValDriveStatus::GetText(char *) const,
//            gcValHeadset::AssignCopy(const cBase *)

class cBase;
class cType;
class cFile;
class cMemPool;

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
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

extern "C" void gcEntityController_gcEntityController(void *, cBase *);

extern char gcRigidBodyControllervirtualtable[];

// ─────────────────────────────────────────────────────────────────────────
// Function 1: gcRigidBodyController::OnDeactivated(void) @ 0x001563d0
// ─────────────────────────────────────────────────────────────────────────

class gcEntityController {
public:
    char _pad[0x8C];
    gcEntityController(cBase *);
    ~gcEntityController();
    void OnDeactivated(void);
    void SetPhysicsController(const cType *);
    void Write(cFile &) const;
};

class gcRigidBodyController : public gcEntityController {
public:
    gcRigidBodyController(cBase *);
    ~gcRigidBodyController();
    void OnDeactivated(void);
    void Write(cFile &) const;
    static gcRigidBodyController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

void gcRigidBodyController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}

// ─────────────────────────────────────────────────────────────────────────
// gcRigidBodyController::Write(cFile &) const @ 0x00156010
// ─────────────────────────────────────────────────────────────────────────

void gcRigidBodyController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcEntityController *)this)->Write(file);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcRigidBodyController::~gcRigidBodyController(void) @ 0x0031f7c0
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcEntityController() (with flags=0), and the
// deleting-tail dispatch through operator delete.
// ─────────────────────────────────────────────────────────────────────────

gcRigidBodyController::~gcRigidBodyController() {
    *(void **)((char *)this + 4) = gcRigidBodyControllervirtualtable;
}

// ─────────────────────────────────────────────────────────────────────────
// gcRigidBodyController::New(cMemPool *, cBase *) static @ 0x0031f618
// ─────────────────────────────────────────────────────────────────────────

gcRigidBodyController *gcRigidBodyController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcRigidBodyController *result = 0;
    gcRigidBodyController *obj = (gcRigidBodyController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController_gcEntityController(obj, parent);
        *(void **)((char *)obj + 4) = gcRigidBodyControllervirtualtable;
        result = obj;
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────
// Function 2: cLanguageSubscriber::Attach(void) @ 0x001c756c
// ─────────────────────────────────────────────────────────────────────────

class cListSubscriber {
public:
    void Attach(void);
};

class cLanguage {
public:
    enum gcSubscription {};
    static void Subscribe(gcSubscription, cListSubscriber *);
};

class cLanguageSubscriber : public cListSubscriber {
public:
    void Attach(void);
};

void cLanguageSubscriber::Attach(void) {
    cListSubscriber::Attach();
    cLanguage::Subscribe((cLanguage::gcSubscription)0, this);
}

// ─────────────────────────────────────────────────────────────────────────
// Function 3: gcState::GetName(char *) const @ 0x002591ec
// ─────────────────────────────────────────────────────────────────────────

void cStrCopy(char *, const char *);

class gcState {
public:
    const char *GetName(void) const;
    void GetName(char *) const;
};

void gcState::GetName(char *buf) const {
    cStrCopy(buf, GetName());
}

// ─────────────────────────────────────────────────────────────────────────
// Function 4: gcValDriveStatus::GetText(char *) const @ 0x00325fac
// ─────────────────────────────────────────────────────────────────────────

void cStrAppend(char *, const char *, ...);

extern const char gcValDriveStatus_fmt[];   // @ 0x36F1D0
extern const char gcValDriveStatus_text[];  // @ 0x36DAF0

class gcValDriveStatus {
public:
    void GetText(char *) const;
};

void gcValDriveStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValDriveStatus_fmt, gcValDriveStatus_text);
}

// ─────────────────────────────────────────────────────────────────────────
// Function 5: gcValHeadset::AssignCopy(const cBase *) @ 0x00346e98
// ─────────────────────────────────────────────────────────────────────────

class gcValHeadset {
public:
    void AssignCopy(const cBase *);
    int pad0;
    int pad4;
    int f8;
};

gcValHeadset *dcast(const cBase *);

void gcValHeadset::AssignCopy(const cBase *base) {
    gcValHeadset *other = dcast(base);
    this->f8 = other->f8;
}
