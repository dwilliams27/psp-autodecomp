// gcDoPortalActivate.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x00319534 gcDoPortalActivate::New(cMemPool *, cBase *)   192B
//   0x0031981c gcDoPortalActivate::Write(cFile &) const       120B

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

// gcAction layout (matches gcAction.cpp).
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

// Dispatch slot in gcDesiredObject's secondary vtable (at this+0x10).
typedef void (*EntityWriteFn)(cBase *, cFile *);

struct EntityWriteSlot {
    short          mOffset;     // +0
    short          _pad;        // +2
    EntityWriteFn  mFn;         // +4
};

struct EntityTypeInfoWrite {
    char            _pad[0x28];
    EntityWriteSlot mSlot;       // +0x28
};

class gcDoPortalActivate : public gcAction {
public:
    // 0x0C: gcDesiredObject sub-object (vtable)
    // 0x10: gcDesiredObject secondary vtable (EntityTypeInfoWrite *)
    // 0x18: int (set to 7 in ctor)
    // 0x1C: int (set to 0 in ctor)
    // 0x20: bool
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

// External constructors / vtables.
extern "C" {
    void gcAction_gcAction(gcDoPortalActivate *, cBase *);
    void gcDesiredObject_gcDesiredObject(void *, void *);
}
extern char gcDoPortalActivatevirtualtable[];
extern char gcDoPortalActivate_desobj_vtable[];
extern char gcDoPortalActivate_vtable1[];

struct PoolBlock {
    char  _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

// ── gcDoPortalActivate::New @ 0x00319534 ──
cBase *gcDoPortalActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPortalActivate *result = 0;
    gcDoPortalActivate *obj = (gcDoPortalActivate *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPortalActivatevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoPortalActivate_desobj_vtable;
        ((int *)obj)[6] = 7;
        ((int *)obj)[7] = 0;
        ((void **)obj)[4] = gcDoPortalActivate_vtable1;
        ((bool *)obj)[0x20] = true;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoPortalActivate::Write @ 0x0031981c ──
void gcDoPortalActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(*(const bool *)((char *)this + 0x20));

    EntityTypeInfoWrite *ti = *(EntityTypeInfoWrite **)((char *)this + 0x10);
    EntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x0C);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}
