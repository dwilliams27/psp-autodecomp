class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    int mField0;
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];

    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct GetTextRec {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntitySetScalevirtualtable[];

class gcDoEntitySetScale : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;

    char _pad0C[0x2C];        // 0x0C
    gcDesiredValue mDesired;  // 0x38
    int mScaleType;           // 0x3C
};

class gcDoSetEventEnumParam : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySetScale asm("D_0009F660");

// 0x002cbb14 - gcDoEntitySetScale::GetType(void) const
const cType *gcDoEntitySetScale::GetType(void) const {
    if (!type_gcDoEntitySetScale) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntitySetScale = cType::InitializeType(
            0, 0, 0xCD, type_action, gcDoEntitySetScale::New, 0, 0, 0);
    }
    return type_gcDoEntitySetScale;
}

// 0x002cba0c - gcDoEntitySetScale::New(cMemPool *, cBase *) static
cBase *gcDoEntitySetScale::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySetScale *result = 0;
    gcDoEntitySetScale *obj =
        (gcDoEntitySetScale *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntitySetScalevirtualtable;

        char *desired = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[4] = D_00000338;

        char *helper = (char *)obj + 0x18;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[4] = (void *)0x388A48;
        ((void **)obj)[9] = (void *)0x37E6A8;
        ((void **)obj)[8] = desired;
        ((void **)obj)[9] = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x28) = one;
        *(unsigned char *)((char *)obj + 0x29) = 0;
        *(int *)((char *)obj + 0x2C) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = (int)desired | 1;
        *(int *)((char *)obj + 0x38) = (int)obj | 1;
        *(int *)((char *)obj + 0x3C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002cbc2c - gcDoEntitySetScale::Write(cFile &) const
void gcDoEntitySetScale::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (void *)((const char *)this + 0x0C);
    rec->fn((char *)base + off, wb._file);

    mDesired.Write(wb);
    wb.Write(mScaleType);
    wb.End();
}

// 0x002cbcb0 - gcDoEntitySetScale::Read(cFile &, cMemPool *)
int gcDoEntitySetScale::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);
    if (rb[3] != 4 || ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *typeInfo = *(char **)((char *)this + 0x10);
    char *base = (char *)this + 0x0C;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short off = rec->offset;
    cFile *f = *(cFile **)&rb[0];
    rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));

    ((gcDesiredValue *)((char *)this + 0x38))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x3C, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x002fc558 - gcDoSetEventEnumParam::Read(cFile &, cMemPool *)
int gcDoSetEventEnumParam::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3U || (unsigned int)rb[3] < 1U ||
        ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *typeInfo = *(char **)((char *)this + 0x10);
    char *base = (char *)this + 0x0C;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short off = rec->offset;
    cFile *f = *(cFile **)&rb[0];
    rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));

    if ((unsigned int)rb[3] >= 2U) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x24, 4);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x002cbf60 - gcDoEntitySetScale::GetText(char *) const
void gcDoEntitySetScale::GetText(char *buf) const {
    char *out = buf;
    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    char local[256];
    local[0] = '\0';
    GetTextRec *rec0 = (GetTextRec *)(typeInfo0 + 0x78);
    char *desired = (char *)this + 0x0C;
    rec0->fn(desired + rec0->offset, out);

    int val = *(int *)((const char *)this + 0x38);
    int isOwned = 0;
    if (val & 1) {
        isOwned = 1;
    }
    void *ptr;
    if (isOwned != 0) {
        ptr = 0;
    } else {
        ptr = (void *)val;
    }

    void *textObj = ptr;
    if (textObj != 0) {
        GetTextRec *rec = (GetTextRec *)(*(char **)((char *)textObj + 4) + 0xD0);
        short off = rec->offset;
        rec->fn((char *)textObj + off, local);
    } else {
        cStrCat(local, (const char *)0x36DB24);
    }

    cStrAppend(out, (const char *)0x36EAC4, (const char *)0x36DAF0, local);
}
