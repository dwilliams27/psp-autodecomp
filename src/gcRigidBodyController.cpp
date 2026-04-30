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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

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
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcRigidBodyController : public gcEntityController {
public:
    gcRigidBodyController(cBase *);
    ~gcRigidBodyController();
    void AssignCopy(const cBase *);
    void OnDeactivated(void);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static gcRigidBodyController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

struct gcRigidBodyControllerBlock36 {
    int unk0;
    unsigned char unk4;
    unsigned char unk5;
    short _pad;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
};

struct gcRigidBodyControllerHalf3 {
    short unk0;
    short unk2;
    short unk4;
};

struct gcRigidBodyControllerLayout {
    char _pad0[8];
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    unsigned char unk24;
    unsigned char unk25;
    short _pad26;
    gcRigidBodyControllerBlock36 unk28[2];
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    gcRigidBodyControllerHalf3 unk80[1];
    float unk88;
    int unk8C;
};

extern "C" void *dcastdcast_gcRigidBodyControllerptr__constcBaseptr(const cBase *);

void gcRigidBodyController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}

// ─────────────────────────────────────────────────────────────────────────
// gcRigidBodyController::Read(cFile &, cMemPool *) @ 0x0015605c
// ─────────────────────────────────────────────────────────────────────────

int gcRigidBodyController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
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
// gcRigidBodyController::AssignCopy(const cBase *) @ 0x0031f488
// ─────────────────────────────────────────────────────────────────────────

void gcRigidBodyController::AssignCopy(const cBase *base) {
    gcRigidBodyControllerLayout *arg0 = (gcRigidBodyControllerLayout *)this;
    gcRigidBodyControllerLayout *temp_v0 =
        (gcRigidBodyControllerLayout *)dcastdcast_gcRigidBodyControllerptr__constcBaseptr(base);
    int *temp_a1 = &temp_v0->unk8;
    int *temp_a3 = &arg0->unk8;

    int temp_unk8 = temp_a1[0];
    int temp_unkC = temp_a1[1];
    int temp_unk10 = temp_a1[2];
    temp_a3[0] = temp_unk8;
    int temp_unk14 = temp_a1[3];
    temp_a3[1] = temp_unkC;
    int temp_unk18 = temp_a1[4];
    temp_a3[2] = temp_unk10;
    int temp_unk1C = temp_a1[5];
    temp_a3[3] = temp_unk14;
    temp_a3[4] = temp_unk18;
    temp_a3[5] = temp_unk1C;

    arg0->unk20 = temp_v0->unk20;
    arg0->unk24 = temp_v0->unk24;
    arg0->unk25 = temp_v0->unk25;

    unsigned int var_a3 = 0;
    gcRigidBodyControllerBlock36 *var_a2 = arg0->unk28;
    gcRigidBodyControllerBlock36 *var_a1 = temp_v0->unk28;
    do {
        int temp_unk0 = var_a1->unk0;
        var_a2->unk0 = temp_unk0;
        unsigned char temp_unk4 = var_a1->unk4;
        var_a2->unk4 = temp_unk4;
        unsigned char temp_unk5 = var_a1->unk5;
        int *temp_t1 = &var_a1->unk8;
        var_a2->unk5 = temp_unk5;
        int *temp_t2 = &var_a2->unk8;
        int temp_unk8_2 = *temp_t1;
        *temp_t2 = temp_unk8_2;
        int *temp_t0 = (int *)((char *)var_a2 + 12);
        int *temp_t1_2 = (int *)((char *)var_a1 + 12);
        int temp_unkC_2 = *temp_t1_2;
        int *temp_t2_2 = (int *)((char *)var_a1 + 16);
        *temp_t0 = temp_unkC_2;
        int *temp_t0_2 = (int *)((char *)var_a2 + 16);
        int *temp_t2_3 = (int *)((char *)var_a1 + 20);
        int temp_unk10_2 = *temp_t2_2;
        int *temp_t0_3 = (int *)((char *)var_a2 + 20);
        *temp_t0_2 = temp_unk10_2;
        int *temp_t2_4 = (int *)((char *)var_a1 + 24);
        int temp_unk14_2 = *temp_t2_3;
        int *temp_t0_4 = (int *)((char *)var_a2 + 24);
        int *temp_t2_5 = (int *)((char *)var_a1 + 28);
        *temp_t0_3 = temp_unk14_2;
        int temp_unk18_2 = *temp_t2_4;
        int *temp_t0_5 = (int *)((char *)var_a2 + 28);
        *temp_t0_4 = temp_unk18_2;
        int temp_unk1C_2 = *temp_t2_5;
        *temp_t0_5 = temp_unk1C_2;
        int temp_unk20_2 = var_a1->unk20;
        var_a3 += 1;
        var_a2->unk20 = temp_unk20_2;
        var_a1 = (gcRigidBodyControllerBlock36 *)((char *)var_a1 + 0x24);
        var_a2 = (gcRigidBodyControllerBlock36 *)((char *)var_a2 + 0x24);
    } while (var_a3 < 2U);

    arg0->unk70 = temp_v0->unk70;
    arg0->unk74 = temp_v0->unk74;

    int *var_a2_2 = &temp_v0->unk78;
    int *var_t0 = &arg0->unk78;
    *var_t0 = *var_a2_2;
    arg0->unk7C = temp_v0->unk7C;

    int var_a3_2 = 0;
    gcRigidBodyControllerHalf3 *var_a2_3 = arg0->unk80;
    gcRigidBodyControllerHalf3 *var_a1_2 = temp_v0->unk80;
    do {
        short temp_unk0_2 = var_a1_2->unk0;
        short temp_unk2_2 = var_a1_2->unk2;
        short temp_unk4_2 = var_a1_2->unk4;
        var_a2_3->unk0 = temp_unk0_2;
        var_a2_3->unk2 = temp_unk2_2;
        var_a2_3->unk4 = temp_unk4_2;
        var_a3_2 += 1;
        var_a2_3 = (gcRigidBodyControllerHalf3 *)((char *)var_a2_3 + 6);
        var_a1_2 = (gcRigidBodyControllerHalf3 *)((char *)var_a1_2 + 6);
    } while (var_a3_2 <= 0);

    float temp_f12 = temp_v0->unk88;
    int *temp_a0_2 = (int *)((char *)temp_v0 + 0x8C);
    arg0->unk88 = temp_f12;
    int *temp_a1_2 = (int *)((char *)arg0 + 0x8C);
    *temp_a1_2 = *temp_a0_2;
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
