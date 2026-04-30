// gcDoSetEventEnumParam.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x002fc304  gcDoSetEventEnumParam::New(cMemPool *, cBase *) static  196B
//   0x002fc4e0  gcDoSetEventEnumParam::Write(cFile &) const             120B

class cBase;
class cFile;
class cMemPool;
class cType;

extern "C" void gcAction_gcAction(void *, cBase *);

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

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
    void Write(int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase        *mOwner;       // 0x00
    void         *mVTable;      // 0x04
    unsigned int  mNext;        // 0x08

    void Write(cFile &) const;
};

class gcDoSetEventEnumParam : public gcAction {
public:
    // 0x0C: embedded gcDesiredEnumerationEntry sub-object
    //   +0x00 (= outer 0x0C): parent back-ref
    //   +0x04 (= outer 0x10): vtable pointer (initially cBase, overwritten)
    //   +0x08 (= outer 0x14): byte flag = 1
    //   +0x09 (= outer 0x15): byte = 0
    //   +0x0C (= outer 0x18): 0
    //   +0x10 (= outer 0x1C): 0
    //   +0x14 (= outer 0x20): (parent | 1)
    // 0x24: int field (set to 0)
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
};

extern char gcDoSetEventEnumParamvirtualtable[];
extern const char gcDoSetEventEnumParam_base_name[] asm("D_0036D894");
extern const char gcDoSetEventEnumParam_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_cNamed asm("D_000385E0");
static cType *type_cObject asm("D_000385E4");
static cType *type_gcDoSetEventEnumParam asm("D_0009F6E4");

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// Type-info slot used for the embedded sub-object's virtual Write dispatch.
typedef void (*EntryWriteFn)(cBase *, cFile *);

struct EntryWriteSlot {
    short        mOffset;     // +0
    short        _pad;        // +2
    EntryWriteFn mFn;         // +4
};

struct EntryTypeInfo {
    char           _pad[0x28];
    EntryWriteSlot mSlot;     // +0x28
};

struct TypeSlot {
    short offset;
    short pad;
    const cType *(*fn)(void *);
};

class gcDesiredEnumerationEntry {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

// ============================================================
// 0x002fc4e0 — Write(cFile &) const, 120B
// ============================================================
void gcDoSetEventEnumParam::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    EntryTypeInfo *ti = *(EntryTypeInfo **)((char *)this + 16);
    EntryWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 12);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(((int *)this)[9]);
    wb.End();
}

// ============================================================
// 0x002fc3c8 — GetType(void) const, 280B
// ============================================================
const cType *gcDoSetEventEnumParam::GetType(void) const {
    if (!type_gcDoSetEventEnumParam) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoSetEventEnumParam_base_name,
                        gcDoSetEventEnumParam_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoSetEventEnumParam = cType::InitializeType(
            0, 0, 0x154, type_action, gcDoSetEventEnumParam::New, 0, 0, 0);
    }
    return type_gcDoSetEventEnumParam;
}

// ============================================================
// 0x002fc970 — VisitReferences(...), 500B
// ============================================================
void gcDoSetEventEnumParam::VisitReferences(
    unsigned int flags,
    cBase *ctx,
    void (*cb)(cBase *, unsigned int, void *),
    void *user,
    unsigned int mask)
{
    gcDesiredEnumerationEntry *entry =
        (gcDesiredEnumerationEntry *)((char *)this + 0xC);

    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    if (entry != 0) {
        gcDesiredEnumerationEntry *typedEntry = 0;

        if (!type_cObject) {
            if (!type_cNamed) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoSetEventEnumParam_base_name,
                        gcDoSetEventEnumParam_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_cNamed = cType::InitializeType(
                    0, 0, 2, type_base,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                    0, 0, 0);
            }
            type_cObject = cType::InitializeType(
                0, 0, 3, type_cNamed, 0, 0, 0, 0);
        }

        void *vt = *(void **)((char *)this + 0x10);
        TypeSlot *typeSlot = (TypeSlot *)((char *)vt + 8);
        const cType *entryType = typeSlot->fn((char *)entry + typeSlot->offset);
        int ok = 0;
        if (type_cObject != 0 && entryType != 0) {
            const cType *node = entryType;
            do {
                if (node == type_cObject) {
                    ok = 1;
                    break;
                }
                node = node->mParent;
            } while (node != 0);
        }
        if (ok != 0) {
            typedEntry = entry;
        }

        if (typedEntry == 0 || (((flags & 0xFE00) &
                                 *(unsigned short *)((char *)typedEntry + 0x28)) == 0)) {
            entry->VisitReferences(flags, (cBase *)this, cb, user, mask);
        } else if (cb != 0) {
            cb((cBase *)this, (unsigned int)(void *)entry, user);
        }
    }
}

// ============================================================
// 0x002fc304 — New(cMemPool *, cBase *) static, 196B
// ============================================================
cBase *gcDoSetEventEnumParam::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetEventEnumParam *result = 0;
    gcDoSetEventEnumParam *obj =
        (gcDoSetEventEnumParam *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSetEventEnumParamvirtualtable;
        // Embedded gcDesiredEnumerationEntry construction at this+0xC:
        ((void **)obj)[4] = (void *)0x37E6A8;             // inner[1] = cBase classdesc
        ((void **)obj)[3] = obj;                           // inner[0] = parent (= outer self)
        ((void **)obj)[4] = (void *)0x388568;             // inner[1] = gcDesiredEnumerationEntryvirtualtable
        *((char *)obj + 0x14) = 1;                         // inner[+0x8] byte
        *((char *)obj + 0x15) = 0;                         // inner[+0x9] byte
        ((int *)obj)[6] = 0;                               // inner[+0xC]
        ((int *)obj)[7] = 0;                               // inner[+0x10]
        ((int *)obj)[8] = (int)((unsigned int)obj | 1);   // inner[+0x14] = (this | 1)
        ((int *)obj)[9] = 0;                               // outer field at 0x24
        result = obj;
    }
    return (cBase *)result;
}
