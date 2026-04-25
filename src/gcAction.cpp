class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

extern "C" {
    void cStrCat(char *, const char *);
    void *cMemPool_GetPoolFromPtr(void *);
    void free(void *);
}

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class gcExpression {
public:
    void GetTextRecursive(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    ~gcAction();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
    const cType *GetType(void) const;
    unsigned int GetTextColor(void) const;
    void GetTextRecursive(char *) const;
    void SetNext(gcExpression *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

extern char gcActionvirtualtable[];
extern const char gcAction_spaceStr[];
extern const char gcAction_typeName[];
extern const char gcAction_typeDesc[];

static cType *gcAction_typeAction;
static cType *gcAction_typeExpression;
static cType *gcAction_typeBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

gcAction::gcAction(cBase *b) {
    mOwner = b;
    mVTable = gcActionvirtualtable;
    mNext = 0;
}

unsigned int gcAction::GetTextColor(void) const {
    volatile unsigned int hl;
    volatile unsigned int def;
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    if (flag) {
        hl = 0xFF808080;
        return hl;
    }
    def = 0xFF000000;
    unsigned int result;
    __asm__ volatile(
        "addiu $v0, $sp, 4\n\t"
        "lw %0, 0($v0)"
        : "=&v"(result)
    );
    return result;
}

void gcAction::GetTextRecursive(char *out) const {
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    if (flag) {
        cStrCat(out, gcAction_spaceStr);
    }
    gcExpression::GetTextRecursive(out);
}

void gcAction::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcExpression::Write(file);
    int flag = 0;
    if (mNext & 1) {
        flag = 1;
    }
    wb.Write(flag);
    wb.End();
}

gcAction::~gcAction() {
    mVTable = gcActionvirtualtable;
}

void gcAction::SetNext(gcExpression *next) {
    mNext = ((unsigned int)next) | (mNext & 3);
}

const cType *gcAction::GetType(void) const {
    if (!gcAction_typeAction) {
        if (!gcAction_typeExpression) {
            if (!gcAction_typeBase) {
                gcAction_typeBase = cType::InitializeType(gcAction_typeName, gcAction_typeDesc, 1, 0, 0, 0, 0, 0);
            }
            gcAction_typeExpression = cType::InitializeType(0, 0, 0x6A, gcAction_typeBase, 0, 0, 0, 0);
        }
        gcAction_typeAction = cType::InitializeType(0, 0, 0x6B, gcAction_typeExpression, 0, 0, 0, 0);
    }
    return gcAction_typeAction;
}

int gcAction::Read(cFile &file, cMemPool *pool) {
    gcAction *self = this;
    int ok = 1;
    register cMemPool *poolPtr = pool;
    register cFile *filePtr = &file;
    cReadBlock rb(file, 3, true);
    if (rb._data[3] == ok + 2 && self->gcExpression::Read(*filePtr, poolPtr)) {
        char selected;
        cFileSystem::Read(*(cFileHandle **)rb._data[0], &selected, 1);
        int selectedFlag = selected != 0;
        selectedFlag = (unsigned char)selectedFlag;
        if ((unsigned char)selectedFlag) {
            self->mNext |= 1;
        } else {
            self->mNext &= ~1u;
        }
        return ok;
    }
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
}

class eShape;

class eCompoundShape {
public:
    char _pad[0x80];
    int *mSubShapes;

    int GetNumSubShapes(void) const;
};

int eCompoundShape::GetNumSubShapes(void) const {
    int count = 0;
    __asm__ volatile("" ::: "memory");
    int *subShapes = mSubShapes;
    if (subShapes != 0) {
        count = subShapes[-1];
    }
    return count;
}
