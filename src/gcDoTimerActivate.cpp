// gcDoTimerActivate.cpp - gcAll_psp.obj
//
// Functions in this file:
//   0x00307cc0 gcDoTimerActivate::New(cMemPool *, cBase *) static  196B
//   0x00307e9c gcDoTimerActivate::Write(cFile &) const             132B

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;       // +0
    int    _pos;        // +4
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
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

typedef void (*TimerWriteFn)(cBase *, cFile *);

struct TimerWriteSlot {
    short        mOffset;     // +0
    short        _pad;        // +2
    TimerWriteFn mFn;         // +4
};

struct TimerTypeInfoWrite {
    char           _pad[0x28];
    TimerWriteSlot mSlot;     // +0x28
};

class gcDoTimerActivate : public gcAction {
public:
    // 0x0C: gcDesiredObject sub-object
    // 0x10: gcDesiredObject secondary vtable (TimerTypeInfoWrite *)
    // 0x18: int (set to 1 in ctor)
    // 0x1C: int (set to 0 in ctor)
    // 0x20: bool
    // 0x21: bool
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

extern "C" {
    void gcAction_gcAction(gcDoTimerActivate *, cBase *);
    void gcDesiredObject_gcDesiredObject(void *, void *);
}

extern char gcDoTimerActivatevirtualtable[];
extern char gcDoTimerActivate_desobj_vtable[];
extern char gcDoTimerActivate_vtable1[];

struct PoolBlock {
    char  _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

// 0x00307cc0 - gcDoTimerActivate::New(cMemPool *, cBase *) static
cBase *gcDoTimerActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTimerActivate *result = 0;
    gcDoTimerActivate *obj =
        (gcDoTimerActivate *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoTimerActivatevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoTimerActivate_desobj_vtable;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((void **)obj)[4] = gcDoTimerActivate_vtable1;
        ((bool *)obj)[0x20] = true;
        ((bool *)obj)[0x21] = false;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00307e9c - gcDoTimerActivate::Write(cFile &) const
void gcDoTimerActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    TimerTypeInfoWrite *ti = *(TimerTypeInfoWrite **)((char *)this + 0x10);
    TimerWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x0C);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(*(const bool *)((char *)this + 0x20));
    wb.Write(*(const bool *)((char *)this + 0x21));
    wb.End();
}
