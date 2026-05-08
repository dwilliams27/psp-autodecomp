// gcDoUISetSprite - gcAll_psp.obj
//   0x00310560 gcDoUISetSprite::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cFileHandle;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
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
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcDesiredUIWidgetHelper {
public:
    void GetText(char *) const;
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcStringLValue {
public:
    void Write(cFile &) const;
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);
extern "C" void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

extern char gcDoUISetSpritevirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];
extern const char gcDoUISetSprite_base_name[] asm("D_0036D894");
extern const char gcDoUISetSprite_base_desc[] asm("D_0036D89C");

class gcDoUISetSprite {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    void Write(cFile &) const;
};

struct ReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoUISetTextSprite {
public:
    void Write(cFile &) const;
};

class gcProfileString {
public:
    void Write(cFile &) const;
};

struct WriteSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUISetSprite asm("D_0009F740");

// 0x00310560 - gcDoUISetSprite::New(cMemPool *, cBase *) static
cBase *gcDoUISetSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetSprite *result = 0;
    gcDoUISetSprite *obj =
        (gcDoUISetSprite *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISetSpritevirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0x0C, 1);
        ((int *)obj)[6] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x1C, obj);
        ((void **)obj)[8] = D_000006F8;
        ((int *)obj)[10] = 7;
        ((int *)obj)[11] = 0;
        ((void **)obj)[8] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00310628 - gcDoUISetSprite::GetType(void) const
const cType *gcDoUISetSprite::GetType(void) const {
    if (!type_gcDoUISetSprite) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoUISetSprite_base_name,
                        gcDoUISetSprite_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUISetSprite = cType::InitializeType(
            0, 0, 0x16F, type_action, gcDoUISetSprite::New, 0, 0, 0);
    }
    return type_gcDoUISetSprite;
}

// 0x00310740 - gcDoUISetSprite::Write(cFile &) const
void gcDoUISetSprite::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcAction *)this)->Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x18));

    char *typeInfo = *(char **)((const char *)this + 0x20);
    WriteSlot *slot = (WriteSlot *)(typeInfo + 0x28);
    void *base = (void *)((char *)this + 0x1C);
    slot->fn((char *)base + slot->offset, wb._file);

    wb.End();
}

// 0x003107c4 - gcDoUISetSprite::Read(cFile &, cMemPool *)
int gcDoUISetSprite::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcAction *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Read(rb);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x18, 4);
    {
        register char *typeInfo __asm__("$5") = *(char **)((char *)this + 0x20);
        register void *base __asm__("$4") = (char *)this + 0x1C;
        ReadSlot *slot = (ReadSlot *)(typeInfo + 0x30);
        short off = slot->offset;
        cFile *f = *(cFile **)&rb._data[0];
        slot->fn((char *)base + off, f, cMemPool::GetPoolFromPtr(base));
    }
    return result;
}

// 0x00310ac0 - gcDoUISetSprite::GetText(char *) const
void gcDoUISetSprite::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->GetText(local);

    cStrAppend(buf, (const char *)0x36F0B0, local, (const char *)0x36DAF0);

    char *typeInfo = *(char **)((const char *)this + 0x20);
    GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0x78);
    void *base = (void *)((char *)this + 0x1C);
    slot->fn((char *)base + slot->offset, buf);

    cStrCat(buf, (const char *)0x36DCEC);
}

// 0x003157f4 - gcDoUISetTextSprite::Write(cFile &) const
void gcDoUISetTextSprite::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcAction *)this)->Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);

    char *typeInfo = *(char **)((const char *)this + 0x20);
    WriteSlot *slot = (WriteSlot *)(typeInfo + 0x28);
    void *base = (void *)((char *)this + 0x1C);
    slot->fn((char *)base + slot->offset, wb._file);

    wb.End();
}

// 0x00286978 - gcProfileString::Write(cFile &) const
void gcProfileString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcStringLValue *)this)->Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x0C));

    char *typeInfo = *(char **)((const char *)this + 0x14);
    WriteSlot *slot = (WriteSlot *)(typeInfo + 0x28);
    void *base = (void *)((char *)this + 0x10);
    slot->fn((char *)base + slot->offset, wb._file);

    wb.End();
}
