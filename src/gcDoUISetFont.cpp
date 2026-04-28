// gcDoUISetFont.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction.
//
// Functions:
//   0x0030f438 gcDoUISetFont::Write(cFile &) const   100B
//   0x0030fba8 gcDoUISetFont::~gcDoUISetFont(void)   124B  (deleting destructor auto-gen)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];   // mOwner, mVTable, mNext at 0/4/8

    gcAction(cBase *);
    ~gcAction();
    void Write(cFile &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];   // 3 ints
    void Write(cWriteBlock &) const;
};

class gcDoUISetFont : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;  // 0x0C
    cHandle                 mHandle;  // 0x18

    gcDoUISetFont(cBase *);
    ~gcDoUISetFont();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcDoUISetFontvirtualtable[];
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcDoUISetFont::New @ 0x0030f288 ──
cBase *gcDoUISetFont::New(cMemPool *pool, cBase *parent) {
    gcDoUISetFont *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetFont *obj = (gcDoUISetFont *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetFontvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0xC, 1);
        *(int *)((char *)obj + 0x18) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoUISetFont::Write @ 0x0030f438 ──
void gcDoUISetFont::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    mWidget.Write(wb);
    mHandle.Write(wb);
    wb.End();
}

// ── gcDoUISetFont::~gcDoUISetFont @ 0x0030fba8 ──
// SNC auto-generates: (this != 0) guard, ~gcAction(0) base call, and
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
gcDoUISetFont::~gcDoUISetFont() {
    *(void **)((char *)this + 4) = gcDoUISetFontvirtualtable;
}
