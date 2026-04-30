// gcDoUIListOp — gcAll_psp.obj
//   0x0030ce6c  New(cMemPool *, cBase *) static     (160B)
//   0x0030d024  Write(cFile &) const                (112B)

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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    gcAction(cBase *);
    void Write(cFile &) const;
};

void gcAction_ctor(gcAction *, cBase *);

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
};

void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoUIListOp : public gcAction {
public:
    ~gcDoUIListOp();
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p);
};

extern char gcDoUIListOpvirtualtable[]; // 0x5A78

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void *cMemPool_GetPoolFromPtr(const void *);
void gcAction___dtor_gcAction_void(void *, int);

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUIListOp asm("D_0009F730");

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

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

inline void gcDoUIListOp::operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

// ============================================================
// 0x0030cf0c — GetType(void) const, 280B
// ============================================================
const cType *gcDoUIListOp::GetType(void) const {
    if (!type_gcDoUIListOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUIListOp = cType::InitializeType(
            0, 0, 0x1DE, type_action, gcDoUIListOp::New, 0, 0, 0);
    }
    return type_gcDoUIListOp;
}

// ============================================================
// 0x0030d024 — Write(cFile &) const, 112B
// ============================================================
void gcDoUIListOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 12))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 24))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 28));
    wb.End();
}

// ============================================================
// 0x0030d6bc — GetText(char *) const, 204B
// ============================================================
void gcDoUIListOp::GetText(char *buf) const {
    char widgetText[256];
    char valueText[256];

    widgetText[0] = *widgetText = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 12))->GetText(widgetText);

    valueText[0] = '\0';
    char *valueBuf = valueText;
    int val = ((int *)this)[6];
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
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)textObj + 4) + 0xD0);
        short off = rec->offset;
        rec->fn((char *)textObj + off, valueBuf);
    } else {
        cStrCat(valueBuf, (const char *)0x36DB24);
    }
    cStrAppend(buf, (const char *)0x36F068, widgetText, (const char *)0x36DAF0, valueBuf);
}

// ============================================================
// 0x0030ce6c — New(cMemPool *, cBase *) static, 160B
// ============================================================
cBase *gcDoUIListOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIListOp *result = 0;
    gcDoUIListOp *obj = (gcDoUIListOp *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor((gcAction *)obj, parent);
        ((void **)obj)[1] = gcDoUIListOpvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 12), 1);
        *(int *)((char *)obj + 24) = (int)((unsigned int)obj | 1);
        *(int *)((char *)obj + 28) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0030da58 — ~gcDoUIListOp(void), 212B
// ============================================================
gcDoUIListOp::~gcDoUIListOp() {
    ((void **)this)[1] = gcDoUIListOpvirtualtable;
    if ((char *)this + 0x18 != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        void *ptr = (void *)val;
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && ptr != 0) {
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)ptr + 4) + 0x50);
            short off = rec->offset;
            rec->fn((char *)ptr + off, (void *)3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }
    gcAction___dtor_gcAction_void(this, 0);
}
