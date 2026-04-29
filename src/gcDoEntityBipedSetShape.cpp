class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityBipedSetShape : public gcAction {
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntityBipedSetShapevirtualtable[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityBipedSetShape;

const cType *gcDoEntityBipedSetShape::GetType(void) const {
    if (!type_gcDoEntityBipedSetShape) {
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
        type_gcDoEntityBipedSetShape = cType::InitializeType(
            0, 0, 0x189, type_action,
            (cBase *(*)(cMemPool *, cBase *))&gcDoEntityBipedSetShape::New,
            0, 0, 0);
    }
    return type_gcDoEntityBipedSetShape;
}

cBase *gcDoEntityBipedSetShape::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityBipedSetShape *result = 0;
    gcDoEntityBipedSetShape *obj =
        (gcDoEntityBipedSetShape *)entry->fn(base, 0x50, 0x10, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityBipedSetShapevirtualtable;

        char *sub = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(sub, (cBase *)obj);
        ((void **)obj)[4] = D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x18, 1, 0, 0);
        void *helperDesc = (void *)0x388A48;
        void *baseDesc = (void *)0x37E6A8;
        ((void **)obj)[4] = helperDesc;
        ((void **)obj)[9] = baseDesc;

        __asm__ volatile(
            "lui        $4,0x39\n"
            "mtc1       $0,$f12\n"
            "sw         $16,32($17)\n"
            "addiu      $4,$4,-31384\n"
            "mfc1       $5,$f12\n"
            "sw         $4,36($17)\n"
            "mfc1       $6,$f12\n"
            "sb         $18,40($17)\n"
            "mfc1       $4,$f12\n"
            "sb         $0,41($17)\n"
            "sw         $0,44($17)\n"
            "ori        $7,$16,1\n"
            "sw         $0,48($17)\n"
            "ori        $8,$17,1\n"
            "sw         $7,52($17)\n"
            "sw         $8,56($17)\n"
            "mtv        $4,S120\n"
            "mtv        $5,S121\n"
            "mtv        $6,S122\n"
            "sv.q       C120,0x40($17)\n"
            :
            :
            : "a0", "a1", "a2", "a3", "t0", "memory");

        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntityBipedSetShape::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    TypeDispatchEntry *entry = (TypeDispatchEntry *)(typeInfo + 0x28);
    char *base = (char *)this + 0x0C;
    entry->fn(base + entry->offset, wb._file);

    wb.Write(3, (const float *)((const char *)this + 0x40));
    ((const gcDesiredValue *)((const char *)this + 0x38))->Write(wb);
    wb.End();
}
