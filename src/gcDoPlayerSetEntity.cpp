// gcDoPlayerSetEntity.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x002F1D00 gcDoPlayerSetEntity::Write(cFile &) const     120B
//   0x002F1FBC gcDoPlayerSetEntity::GetText(char *) const    224B

class cBase;
class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;       // +0
    int    _pos;        // +4
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

// gcAction layout (matches gcAction.cpp).
class gcAction : public gcExpression {
public:
    cBase        *mOwner;       // 0x00
    void         *mVTable;      // 0x04
    unsigned int  mNext;        // 0x08

    void Write(cFile &) const;
};

struct PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void cStrAppend(char *, const char *, ...);
    void cStrCat(char *, const char *);
    void gcAction_gcAction(void *, cBase *);
    void gcDesiredObject_gcDesiredObject(void *, cBase *);
    void gcDesiredEntityHelper_ctor(void *, int, int, int)
        __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
}

extern const char gcDoPlayerSetEntity_fmt[];        // 0x36EDC0
extern const char gcDoPlayerSetEntity_emptyStr[];   // 0x36DB24
extern char D_00000338[];
extern char gcDoPlayerSetEntityvirtualtable[];

// Type-info slot layout used for the "GetText" / "Write" sub-object dispatch.
typedef void (*EntityWriteFn)(cBase *, cFile *);
typedef void (*EntityGetTextFn)(cBase *, char *);

struct EntityWriteSlot {
    short          mOffset;     // +0
    short          _pad;        // +2
    EntityWriteFn  mFn;         // +4
};

struct EntityGetTextSlot {
    short            mOffset;
    short            _pad;
    EntityGetTextFn  mFn;
};

struct EntityTypeInfoWrite {
    char            _pad[0x28];
    EntityWriteSlot mSlot;       // +0x28
};

struct EntityTypeInfoGetText {
    char              _pad[0x78];
    EntityGetTextSlot mSlot;     // +0x78
};

struct ParamTypeInfoGetText {
    char              _pad[0xD0];
    EntityGetTextSlot mSlot;     // +0xD0
};

class gcDoPlayerSetEntity : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    // 0x0C: gcDesiredValue
    // 0x10: embedded entity base
    // 0x14: pointer to type info
    void Write(cFile &) const;
    void GetText(char *) const;
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoPlayerSetEntity asm("D_0009F6C8");

// ── gcDoPlayerSetEntity::New @ 0x002F1AE4 ──
cBase *gcDoPlayerSetEntity::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayerSetEntity *result = 0;
    gcDoPlayerSetEntity *obj =
        (gcDoPlayerSetEntity *)entry->fn(base, 0x3C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlayerSetEntityvirtualtable;
        *(int *)((char *)obj + 0x0C) = ((int)obj) | 1;

        char *desired = (char *)obj + 0x10;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[5] = D_00000338;

        char *helper = (char *)obj + 0x1C;
        int one = 1;
        gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
        ((void **)obj)[5] = (void *)0x388A48;
        ((void **)obj)[10] = (void *)0x37E6A8;
        ((void **)obj)[9] = desired;
        ((void **)obj)[10] = (void *)0x388568;
        *(unsigned char *)((char *)obj + 0x2C) = one;
        *(unsigned char *)((char *)obj + 0x2D) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        *(int *)((char *)obj + 0x38) = (int)desired | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoPlayerSetEntity::GetType @ 0x002F1BE8 ──
const cType *gcDoPlayerSetEntity::GetType(void) const {
    if (!type_gcDoPlayerSetEntity) {
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
        type_gcDoPlayerSetEntity = cType::InitializeType(
            0, 0, 0xA7, type_action, gcDoPlayerSetEntity::New, 0, 0, 0);
    }
    return type_gcDoPlayerSetEntity;
}

// ── gcDoPlayerSetEntity::Write @ 0x002F1D00 ──
void gcDoPlayerSetEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    ((const gcDesiredValue *)((char *)this + 12))->Write(wb);

    EntityTypeInfoWrite *ti = *(EntityTypeInfoWrite **)((char *)this + 20);
    EntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 16);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}

// ── gcDoPlayerSetEntity::GetText @ 0x002F1FBC ──
void gcDoPlayerSetEntity::GetText(char *buf) const {
    char paramBuf[256];
    char nameBuf[256];
    paramBuf[0] = '\0';
    nameBuf[0] = '\0';
    char *namePtr = nameBuf;

    int val = ((int *)this)[3];

    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }

    char *eslotP;
    cBase *check = ptr;
    if (check != 0) {
        char *pti = *(char **)((char *)check + 4);
        EntityGetTextSlot *pslot = (EntityGetTextSlot *)(pti + 0xD0);
        pslot->mFn((cBase *)((char *)check + pslot->mOffset), paramBuf);
        eslotP = *(char **)((char *)this + 0x14) + 0x78;
    } else {
        cStrCat(paramBuf, gcDoPlayerSetEntity_emptyStr);
        eslotP = *(char **)((char *)this + 0x14) + 0x78;
    }

    EntityGetTextSlot *eslot = (EntityGetTextSlot *)eslotP;
    cBase *embedded = (cBase *)((char *)this + 0x10);
    eslot->mFn((cBase *)((char *)embedded + eslot->mOffset), namePtr);

    cStrAppend(buf, gcDoPlayerSetEntity_fmt, paramBuf, namePtr);
}
