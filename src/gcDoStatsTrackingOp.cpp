// gcDoStatsTrackingOp — gcAll_psp.obj
//   0x00303908  gcDoStatsTrackingOp::New(cMemPool *, cBase *) static
//   0x00303ac4  gcDoStatsTrackingOp::Write(cFile &) const

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
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoStatsTrackingOp {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void GetText(char *) const;
    void Write(cFile &) const;
    gcDoStatsTrackingOp &operator=(const gcDoStatsTrackingOp &);
};

void gcAction_gcAction(gcDoStatsTrackingOp *, cBase *);
void gcAction_Write(const gcDoStatsTrackingOp *, cFile &);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern char gcDoStatsTrackingOpvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoStatsTrackingOp asm("D_0009F704");

cBase *gcDoStatsTrackingOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoStatsTrackingOp *result = 0;
    gcDoStatsTrackingOp *obj = (gcDoStatsTrackingOp *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoStatsTrackingOpvirtualtable;
        ((int *)obj)[3] = 1;
        int tagged = (int)obj | 1;
        ((int *)obj)[4] = tagged;
        ((int *)obj)[5] = tagged;
        ((int *)obj)[6] = tagged;
        ((int *)obj)[7] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoStatsTrackingOp::GetType(void) const {
    if (!type_gcDoStatsTrackingOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoStatsTrackingOp = cType::InitializeType(
            0, 0, 0xE7, type_action, gcDoStatsTrackingOp::New, 0, 0, 0x80);
    }
    return type_gcDoStatsTrackingOp;
}

void gcDoStatsTrackingOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoStatsTrackingOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoStatsTrackingOp = cType::InitializeType(
                0, 0, 0xE7, type_action, gcDoStatsTrackingOp::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoStatsTrackingOp;
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
    *this = *(const gcDoStatsTrackingOp *)copy;
}

void gcDoStatsTrackingOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);

    int val = ((int *)this)[6];
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
    wb.WriteBase(ptr);

    int val2 = ((int *)this)[7];
    int flag2 = 0;
    if (val2 & 1) {
        flag2 = 1;
    }
    cBase *ptr2;
    if (flag2 != 0) {
        ptr2 = 0;
    } else {
        ptr2 = (cBase *)val2;
    }
    wb.WriteBase(ptr2);
    wb.End();
}

void gcDoStatsTrackingOp::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36EF60, (const char *)0x36DAF0);

    int flag = 0;
    int op = *(int *)((const char *)this + 0x0C);
    if (op == 7 || op == 9 || op == 8) {
        flag = 1;
    }
    int useValue = flag & 0xFF;
    if (useValue != 0) {
        int val = *(int *)((const char *)this + 0x10);
        int owned = 0;
        if (val & 1) {
            owned = 1;
        }
        if (owned != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val + slot->offset, buf);
        } else {
            cStrCat(buf, (const char *)0x36DB24);
        }
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}
