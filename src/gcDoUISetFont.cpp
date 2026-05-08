// gcDoUISetFont.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction.
//
// Functions:
//   0x0030f438 gcDoUISetFont::Write(cFile &) const   100B
//   0x0030f49c gcDoUISetFont::Read(cFile &, cMemPool *) 232B
//   0x0030fba8 gcDoUISetFont::~gcDoUISetFont(void)   124B  (deleting destructor auto-gen)

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
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
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
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cHandle {
public:
    int mIndex;
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];   // 3 ints
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
};

class gcDoUISetFont : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;  // 0x0C
    cHandle                 mHandle;  // 0x18

    gcDoUISetFont(cBase *);
    ~gcDoUISetFont();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
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
gcDoUISetFont *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
extern void *D_00038890[];

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct HandleObject {
    char pad0[0x1C];
    short nameLen;
    char pad1[0x12];
    int handle;
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUISetFont asm("D_0009F738");

// ── gcDoUISetFont::AssignCopy @ 0x0030f208 ──
void gcDoUISetFont::AssignCopy(const cBase *other) {
    gcDoUISetFont *src = dcast(other);
    int *self_i = (int *)this;
    int *src_i = (int *)src;
    int v = self_i[2] & ~3;
    self_i[2] = v;
    self_i[2] = v | (src_i[2] & 3);
    self_i[3] = src_i[3];
    int *s1 = (int *)((char *)src + 0x10);
    int *d1 = (int *)((char *)this + 0x10);
    int *s2 = (int *)((char *)src + 0x14);
    int *d2 = (int *)((char *)this + 0x14);
    *d1 = *s1;
    *d2 = *s2;
    *(cHandle *)((char *)this + 0x18) = *(const cHandle *)((char *)src + 0x18);
}

// ── gcDoUISetFont::GetType @ 0x0030f320 ──
const cType *gcDoUISetFont::GetType(void) const {
    if (!type_gcDoUISetFont) {
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
        type_gcDoUISetFont = cType::InitializeType(
            0, 0, 0xEC, type_action, gcDoUISetFont::New, 0, 0, 0);
    }
    return type_gcDoUISetFont;
}

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

// -- gcDoUISetFont::Read(cFile &, cMemPool *) @ 0x0030f49c --
int gcDoUISetFont::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    mWidget.Read(rb);
    mHandle.mIndex = 0;
    mHandle.Read(rb, cMemPool::GetPoolFromPtr(&mHandle));
    return result;
}

// -- gcDoUISetFont::GetText(char *) const @ 0x0030f788 --
void gcDoUISetFont::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);

    register const char *fmt __asm__("$4") = (const char *)0x36F088;
    register int h __asm__("$5") = *(int *)((char *)this + 0x18);
    register HandleObject *obj __asm__("$6");
    if (h == 0) {
        obj = 0;
    } else {
        register HandleObject *cand __asm__("$7") =
            (HandleObject *)D_00038890[h & 0xFFFF];
        obj = 0;
        if (cand != 0) {
            if (cand->handle == h) {
                obj = cand;
            }
        }
    }

    const char *text;
    if (obj != 0) {
        unsigned int empty = (obj->nameLen == 0);
        empty &= 0xFF;
        if (empty) {
            text = (const char *)0x36DAB8;
            goto object_done;
        }
        text = (const char *)obj + 8;
object_done:
        __asm__ volatile("" : : "r"(text));
        goto append;
    } else if (h != 0) {
        text = (const char *)0x36DAC4;
    } else {
        text = (const char *)0x36DACC;
    }
append:
    cStrAppend(buf, fmt, local, text);
}

// ── gcDoUISetFont::~gcDoUISetFont @ 0x0030fba8 ──
// SNC auto-generates: (this != 0) guard, ~gcAction(0) base call, and
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
gcDoUISetFont::~gcDoUISetFont() {
    *(void **)((char *)this + 4) = gcDoUISetFontvirtualtable;
}
