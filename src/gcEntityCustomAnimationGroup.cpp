class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

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

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cGroup {
public:
    int _pad[2];
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcEntityCustomAnimationGroup : public cGroup {
public:
    unsigned char mFlag;
    int mField;
    char _pad1[3];
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    ~gcEntityCustomAnimationGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcEntityCustomAnimationGroupvirtualtable[];

class eInputJoystick {
public:
    static void RecheckPresent();
};

class eInputMouse {
public:
    static void RecheckPresent();
};

class eInputKeyboard {
public:
    static void RecheckPresent();
};

namespace eInput {
    void RecheckPresent();
}

class gcUI {
public:
    class gcFader {
    public:
        char _pad0[0x24];
        void Reset();
    };

    char _pad0[8];
    gcFader mFader0;
    gcFader mFader1;

    void Reset();
};

class cType;

class gcEntityController {
public:
    void OnDeselected();
    void SetPhysicsController(const cType *);
};

class gcRigidBodyController : public gcEntityController {
public:
    void OnDeselected();
};

class gcValLobbyInfo {
public:
    int _pad4;
    int _pad0;
    int mValue;
    void AssignCopy(const cBase *);
};

void gcEntityCustomAnimationGroup::AssignCopy(const cBase *base) {
    gcEntityCustomAnimationGroup *src = dcast<gcEntityCustomAnimationGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcEntityCustomAnimationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// gcEntityCustomAnimationGroup::Write(cFile &) const @ 0x000d2098
void gcEntityCustomAnimationGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// gcEntityCustomAnimationGroup::Read(cFile &, cMemPool *) @ 0x000d20e4
int gcEntityCustomAnimationGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// gcEntityCustomAnimationGroup::~gcEntityCustomAnimationGroup(void) @ 0x00238080
gcEntityCustomAnimationGroup::~gcEntityCustomAnimationGroup() {
    *(void **)((char *)this + 4) = gcEntityCustomAnimationGroupvirtualtable;
}

void eInput::RecheckPresent() {
    eInputJoystick::RecheckPresent();
    eInputMouse::RecheckPresent();
    eInputKeyboard::RecheckPresent();
}

void gcUI::Reset() {
    mFader0.Reset();
    mFader1.Reset();
}

void gcRigidBodyController::OnDeselected() {
    gcEntityController::OnDeselected();
    gcEntityController::SetPhysicsController(0);
}

void gcValLobbyInfo::AssignCopy(const cBase *base) {
    gcValLobbyInfo *src = dcast<gcValLobbyInfo>(base);
    mValue = src->mValue;
}
