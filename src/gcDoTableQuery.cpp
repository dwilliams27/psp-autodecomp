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
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDoTableQuery : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

typedef void (*WriteFn)(cBase *, cFile *);

struct TypeMethod {
    short offset;
    short pad;
    WriteFn fn;
};

void gcAction_gcAction(gcDoTableQuery *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);

extern char gcActionvirtualtable[];
extern char D_00000838[];
extern const char gcDoTableQuery_base_name[] asm("D_0036D894");
extern const char gcDoTableQuery_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoTableQuery asm("D_0009F714");

cBase *gcDoTableQuery::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTableQuery *result = 0;
    gcDoTableQuery *obj =
        (gcDoTableQuery *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcActionvirtualtable;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x0C, (cBase *)obj);
        ((void **)obj)[4] = D_00000838;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((int *)obj)[4] = 0x38A000;
        ((int *)obj)[8] = 0;
        ((gcDoTableQuery **)obj)[9] = obj;
        ((int *)obj)[10] = 0;
        ((gcDoTableQuery **)obj)[11] = obj;
        gcEvent_gcEvent((char *)obj + 0x30, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x4C, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x68, (cBase *)obj, 0);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoTableQuery::GetType(void) const {
    if (!type_gcDoTableQuery) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoTableQuery_base_name, gcDoTableQuery_base_desc, 1,
                        0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoTableQuery = cType::InitializeType(
            0, 0, 0x21A, type_action, gcDoTableQuery::New, 0, 0, 0);
    }
    return type_gcDoTableQuery;
}

void gcDoTableQuery::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);

    TypeMethod *slot0 = (TypeMethod *)((char *)*(void **)((char *)this + 0x10) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x0C);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);
    ((const cBaseArray *)((const char *)this + 0x20))->Write(wb);
    ((const cBaseArray *)((const char *)this + 0x28))->Write(wb);

    TypeMethod *slot1 = (TypeMethod *)((char *)*(void **)((char *)this + 0x34) + 0x28);
    cBase *base1 = (cBase *)((char *)this + 0x30);
    slot1->fn((cBase *)((char *)base1 + slot1->offset), wb._file);
    TypeMethod *slot2 = (TypeMethod *)((char *)*(void **)((char *)this + 0x50) + 0x28);
    cBase *base2 = (cBase *)((char *)this + 0x4C);
    slot2->fn((cBase *)((char *)base2 + slot2->offset), wb._file);
    TypeMethod *slot3 = (TypeMethod *)((char *)*(void **)((char *)this + 0x6C) + 0x28);
    cBase *base3 = (cBase *)((char *)this + 0x68);
    slot3->fn((cBase *)((char *)base3 + slot3->offset), wb._file);
    wb.End();
}
