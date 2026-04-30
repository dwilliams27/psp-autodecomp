class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
};

class gcDoSetStringValue : public gcAction {
public:
    int mValue;
    int mString;

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoSetStringValue &operator=(const gcDoSetStringValue &);
    void Write(cFile &) const;
};

class gcStatsTrackingStrings {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcDoSetStringValuevirtualtable[];
extern char gcStatsTrackingStringsvirtualtable[];

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
    short _pad;
    const cType *(*getType)(void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_gcDoSetStringValue asm("D_0009F6F4");
static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");

cBase *gcDoSetStringValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetStringValue *result = 0;
    gcDoSetStringValue *obj =
        (gcDoSetStringValue *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcAction(parent);
        ((void **)obj)[1] = gcDoSetStringValuevirtualtable;
        int self = (int)obj | 1;
        ((int *)obj)[3] = self;
        ((int *)obj)[4] = self;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoSetStringValue::GetType(void) const {
    if (!type_gcDoSetStringValue) {
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
        type_gcDoSetStringValue = cType::InitializeType(
            0, 0, 0x180, type_action, gcDoSetStringValue::New, 0, 0, 0);
    }
    return type_gcDoSetStringValue;
}

void gcDoSetStringValue::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoSetStringValue) {
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
            type_gcDoSetStringValue = cType::InitializeType(
                0, 0, 0x180, type_action, gcDoSetStringValue::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoSetStringValue;
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
    *this = *(const gcDoSetStringValue *)copy;
}

void gcDoSetStringValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    int valC = ((const int *)this)[3];
    int flagC = 0;
    if (valC & 1) {
        flagC = 1;
    }
    cBase *ptrC;
    if (flagC != 0) {
        ptrC = 0;
    } else {
        ptrC = (cBase *)valC;
    }
    wb.WriteBase(ptrC);

    int val10 = ((const int *)this)[4];
    int flag10 = 0;
    if (val10 & 1) {
        flag10 = 1;
    }
    cBase *ptr10;
    if (flag10 != 0) {
        ptr10 = 0;
    } else {
        ptr10 = (cBase *)val10;
    }
    wb.WriteBase(ptr10);

    wb.End();
}

cBase *gcStatsTrackingStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStatsTrackingStrings *result = 0;
    gcStatsTrackingStrings *obj =
        (gcStatsTrackingStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcStatsTrackingStringsvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int self = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = self;
        *(int *)((char *)obj + 0x10) = self;
        result = obj;
    }
    return (cBase *)result;
}
