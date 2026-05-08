class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cBase;
class cMemPool;
class cType;
class cFileHandle;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int pos;
    int _data[3];

    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class gcAction {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void Read(cReadBlock &);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcDesiredEntityHelper {
public:
    void Read(cReadBlock &);
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

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);
extern "C" void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
void cStrAppend(char *, const char *, ...);

extern char gcDoUISetStatevirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];
extern const char gcDoUISetState_base_name[] asm("D_0036D894");
extern const char gcDoUISetState_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUISetState asm("D_0009F744");
static cType *type_gcDoUISetTextSprite asm("D_0009F750");

typedef void (*WriteFn)(cBase *, cFile *);

struct TypeMethod {
    short offset;
    short pad;
    WriteFn fn;
};

struct ReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoUISetState : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    gcDoUISetState &operator=(const gcDoUISetState &);
};

class gcDoUISetTextSprite : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoUISetTextSprite &operator=(const gcDoUISetTextSprite &);
};

cBase *gcDoUISetState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetState *result = 0;
    gcDoUISetState *obj = (gcDoUISetState *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISetStatevirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0x0C, 1);
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0x1C) = tagged;
        *(int *)((char *)obj + 0x20) = tagged;
        *(int *)((char *)obj + 0x24) = tagged;
        *(int *)((char *)obj + 0x28) = tagged;
        *(int *)((char *)obj + 0x2C) = tagged;
        *(int *)((char *)obj + 0x30) = tagged;
        *(int *)((char *)obj + 0x34) = tagged;
        *(int *)((char *)obj + 0x38) = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x3C, obj);
        ((void **)obj)[0x40 / 4] = D_000006F8;
        *(int *)((char *)obj + 0x48) = 7;
        *(int *)((char *)obj + 0x4C) = 0;
        ((void **)obj)[0x40 / 4] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoUISetState::GetType(void) const {
    if (!type_gcDoUISetState) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoUISetState_base_name, gcDoUISetState_base_desc, 1,
                        0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUISetState = cType::InitializeType(
            0, 0, 0x1BD, type_action, gcDoUISetState::New, 0, 0, 0);
    }
    return type_gcDoUISetState;
}

void gcDoUISetState::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoUISetState) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoUISetState_base_name, gcDoUISetState_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoUISetState = cType::InitializeType(
                0, 0, 0x1BD, type_action, gcDoUISetState::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoUISetState;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoUISetState *)copy;
}

void gcDoUISetState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0x18));
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x24))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x28))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x2C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x30))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x34))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x38));

    TypeMethod *slot0 =
        (TypeMethod *)((char *)*(void **)((char *)this + 0x40) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x3C);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);

    wb.End();
}

int gcDoUISetState::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[1] != 1 || gcAction::Read(file, pool) == 0) {
        rb.file->SetCurrentPos(rb.pos);
        return 0;
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Read(rb);
    cFileSystem::Read(*(cFileHandle **)rb.file, (char *)this + 0x18, 4);
    ((gcDesiredValue *)((char *)this + 0x1C))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x20))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x24))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x28))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x2C))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x30))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x34))->Read(rb);
    cFileSystem::Read(*(cFileHandle **)rb.file, (char *)this + 0x38, 4);
    {
        register char *typeInfo __asm__("$5") = *(char **)((char *)this + 0x40);
        register void *base __asm__("$4") = (char *)this + 0x3C;
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        cFile *f = rb.file;
        slot->fn((char *)base + off, f, cMemPool::GetPoolFromPtr(base));
    }
    return result;
}

void gcDoUISetState::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->GetText(local);
    cStrAppend(buf, (const char *)0x36F0C4, local);

    unsigned int mask = 1;
    do {
        int enabled = (*(int *)((const char *)this + 0x18) & mask) != 0;
        enabled = enabled & 0xFF;
        if (enabled != 0) {
            cStrAppend(buf, (const char *)0x36DBAC, (const char *)0x36DAF0);
        }
        mask = mask << 1;
    } while (mask != 0);
    cStrAppend(buf, (const char *)0x36DCEC);
}

void gcDoUISetTextSprite::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoUISetTextSprite) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoUISetState_base_name, gcDoUISetState_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoUISetTextSprite = cType::InitializeType(
                0, 0, 0x150, type_action, gcDoUISetTextSprite::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoUISetTextSprite;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoUISetTextSprite *)copy;
}

class gcObjectRelationship {
public:
    int Read(cFile &, cMemPool *);
};

class gcEntityRelationship : public gcObjectRelationship {
public:
    int Read(cFile &, cMemPool *);
};

int gcEntityRelationship::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[1] != 1 || gcObjectRelationship::Read(file, pool) == 0) {
        rb.file->SetCurrentPos(rb.pos);
        return 0;
    }
    ((gcDesiredEntityHelper *)((char *)this + 0x20))->Read(rb);
    return result;
}
