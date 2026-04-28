// gcDoSetEventEnumParam.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x002fc304  gcDoSetEventEnumParam::New(cMemPool *, cBase *) static  196B
//   0x002fc4e0  gcDoSetEventEnumParam::Write(cFile &) const             120B

class cBase;
class cFile;
class cMemPool;

extern "C" void gcAction_gcAction(void *, cBase *);

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
    static cBase *New(cMemPool *, cBase *);
};

extern char gcDoSetEventEnumParamvirtualtable[];

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
