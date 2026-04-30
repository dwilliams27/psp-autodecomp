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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoPlay2dSound {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoPlay2dSound &operator=(const gcDoPlay2dSound &);
};

class gcDoPlayMovie {
public:
    static cBase *New(cMemPool *, cBase *);
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

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
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

extern char gcDoPlay2dSoundvirtualtable[];
extern char gcDoPlayMovievirtualtable[];
extern char D_00000658[];
extern char D_00389800[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPlay2dSound asm("D_0009F6BC");

cBase *gcDoPlay2dSound::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlay2dSound *result = 0;
    gcDoPlay2dSound *obj =
        (gcDoPlay2dSound *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlay2dSoundvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 2;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x14, obj);
        ((void **)obj)[6] = D_00000658;
        ((int *)obj)[8] = 7;
        ((int *)obj)[9] = 0;
        ((void **)obj)[6] = D_00389800;
        ((int *)obj)[10] = (int)((unsigned int)obj | 1);
        ((int *)obj)[11] = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoPlay2dSound::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoPlay2dSound) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoPlay2dSound = cType::InitializeType(
                0, 0, 0xF9, type_action, gcDoPlay2dSound::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoPlay2dSound;
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
    *this = *(const gcDoPlay2dSound *)copy;
}

const cType *gcDoPlay2dSound::GetType(void) const {
    if (!type_gcDoPlay2dSound) {
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
        type_gcDoPlay2dSound = cType::InitializeType(
            0, 0, 0xF9, type_action, gcDoPlay2dSound::New, 0, 0, 0x80);
    }
    return type_gcDoPlay2dSound;
}

void gcDoPlay2dSound::Write(cFile &file) const {
    cWriteBlock wb(file, 9);
    ((const gcAction *)this)->Write(file);
    wb.Write(((const int *)this)[3]);

    char *typeInfo = *(char **)((const char *)this + 0x18);
    WriteEntry *entry = (WriteEntry *)(typeInfo + 0x28);
    short off = entry->offset;
    void *base = (void *)((const char *)this + 0x14);
    entry->fn((char *)base + off, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x28))->Write(wb);
    wb.Write(((const int *)this)[4]);
    ((const gcDesiredValue *)((const char *)this + 0x2C))->Write(wb);
    wb.End();
}

cBase *gcDoPlayMovie::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayMovie *result = 0;
    gcDoPlayMovie *obj =
        (gcDoPlayMovie *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlayMovievirtualtable;
        *(short *)((char *)obj + 0x20) = 0;
        *(short *)((char *)obj + 0x22) = 0;
        int value = (int)((unsigned int)obj | 1);
        *(char *)((char *)obj + 0x0C) = 0;
        *(int *)((char *)obj + 0x24) = value;
        *(char *)((char *)obj + 0x28) = 1;
        *(int *)((char *)obj + 0x2C) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        *(int *)((char *)obj + 0x38) = 0;
        *(int *)((char *)obj + 0x3C) = 0;
        *(int *)((char *)obj + 0x40) = value;
        *(int *)((char *)obj + 0x44) = value;
        *(int *)((char *)obj + 0x48) = value;
        *(int *)((char *)obj + 0x4C) = value;
        *(int *)((char *)obj + 0x50) = value;
        *(int *)((char *)obj + 0x54) = 0;
        result = obj;
    }
    return (cBase *)result;
}
