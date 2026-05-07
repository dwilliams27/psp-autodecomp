class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x28];
    cTypeMethod write_m;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    unsigned int value;

    void Write(cWriteBlock &) const;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *owner;
    void *vtable;
    unsigned int next;

    void Write(cFile &) const;
};

typedef void (*WriteFn)(void *, cFile *);

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

struct DtorRec {
    short offset;
    short pad;
    void (*fn)(void *, int, void *, short);
};

struct CloneRec {
    short offset;
    short pad;
    int (*fn)(void *, cMemPool *, int, void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntitySetPrimaryControllervirtualtable[];

class gcDoEntitySetPrimaryController : public gcAction {
public:
    unsigned int desiredController;
    cType *desiredControllerType;
    char pad14[0x24];
    gcDesiredValue desiredValue;
    bool field_3C;
    bool field_3D;

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

extern const char gcDoEntitySetPrimaryController_base_name[];
extern const char gcDoEntitySetPrimaryController_base_desc[];

cBase *gcDoEntitySetPrimaryController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySetPrimaryController *result = 0;
    gcDoEntitySetPrimaryController *obj =
        (gcDoEntitySetPrimaryController *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntitySetPrimaryControllervirtualtable;

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
        *(unsigned char *)((char *)obj + 0x3C) = 0;
        *(unsigned char *)((char *)obj + 0x3D) = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntitySetPrimaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    const cTypeMethod *slot = &desiredControllerType->write_m;
    char *base = (char *)this + 0x0C;
    ((WriteFn)slot->fn)(base + slot->offset, wb.file);

    desiredValue.Write(wb);
    wb.Write(field_3D);
    wb.Write(field_3C);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySetPrimaryController;

void gcDoEntitySetPrimaryController::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    register char *ownSlot __asm__("$s7") = (char *)this + 0x38;

    if (other != 0) {
        if (!type_gcDoEntitySetPrimaryController) {
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
            type_gcDoEntitySetPrimaryController = cType::InitializeType(
                0, 0, 0x11D, type_action,
                gcDoEntitySetPrimaryController::New, 0, 0, 0);
        }

        void *vt = ((void **)other)[1];
        const cType *target = type_gcDoEntitySetPrimaryController;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short off = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + off);
        int ok;

        if (target == 0) {
            ok = 0;
            goto cast_done;
        }
        if (type != 0) {
        cast_loop:
            if (type == target) {
                ok = 1;
                goto cast_done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto cast_loop;
            }
        }
        ok = 0;
    cast_done:
        if (ok != 0) {
            copy = other;
        }
    }

    int flags = *(int *)((char *)this + 8) & ~3;
    *(int *)((char *)this + 8) = flags;
    int srcBits = *(int *)((const char *)copy + 8) & 3;
    *(int *)((char *)this + 8) = flags | srcBits;
    ((gcDesiredEntity *)((char *)this + 0x0C))->operator=(
        *(const gcDesiredEntity *)((const char *)copy + 0x0C));

    if ((const char *)copy + 0x38 != ownSlot) {
        goto copy_slot;
    }
    goto copy_bytes;

copy_slot:
    {
        int currentWord = *(int *)((char *)this + 0x38);
        int ownsCurrent = 1;
        int currentTag = currentWord & 1;
        if (currentTag != 0) {
            ownsCurrent = 0;
        }
        if (ownsCurrent != 0) {
            int current = currentWord;
            int isTagged = 0;
            if (currentTag != 0) {
                isTagged = 1;
            }
            int replacement;
            if (isTagged != 0) {
                replacement = currentWord & ~1;
                replacement |= 1;
            } else {
                replacement = *(int *)currentWord;
                replacement |= 1;
            }
            *(int *)((char *)this + 0x38) = replacement;
            if (current != 0) {
                char *vt2 = *(char **)(current + 4);
                DtorRec *rec = (DtorRec *)(vt2 + 0x50);
                short doff = rec->offset;
                void (*fn)(void *, int, void *, short) = rec->fn;
                fn((char *)current + doff, 3, (void *)fn, doff);
            }
        }

        int src = *(int *)((const char *)copy + 0x38);
        int ownsSrc = 1;
        int srcTag = src & 1;
        if (srcTag != 0) {
            ownsSrc = 0;
        }
        if (ownsSrc != 0) {
            char *vt3 = *(char **)(src + 4);
            CloneRec *rec = (CloneRec *)(vt3 + 0x10);
            short coff = rec->offset;
            char *srcBase = (char *)src + coff;
            cMemPool *pool = cMemPool::GetPoolFromPtr(ownSlot);
            int old = *(int *)((char *)this + 0x38);
            int oldTagged = 0;
            void *fn = (void *)rec->fn;
            int oldTag = old & 1;
            if (oldTag != 0) {
                oldTagged = 1;
            }
            int oldArg;
            if (oldTagged != 0) {
                oldArg = old & ~1;
            } else {
                oldArg = *(int *)old;
            }
            *(int *)((char *)this + 0x38) =
                rec->fn(srcBase, pool, oldArg, fn);
        }
    }

copy_bytes:
    *(unsigned char *)((char *)this + 0x3C) =
        *(const unsigned char *)((const char *)copy + 0x3C);
    *(unsigned char *)((char *)this + 0x3D) =
        *(const unsigned char *)((const char *)copy + 0x3D);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fegcDoEntitySetPrimaryControllerKAssignCopyPC6FcBase, 0x2e0\n");

const cType *gcDoEntitySetPrimaryController::GetType(void) const {
    if (!type_gcDoEntitySetPrimaryController) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetPrimaryController_base_name,
                        gcDoEntitySetPrimaryController_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetPrimaryController = cType::InitializeType(
            0, 0, 0x11D, type_action,
            gcDoEntitySetPrimaryController::New, 0, 0, 0);
    }
    return type_gcDoEntitySetPrimaryController;
}
