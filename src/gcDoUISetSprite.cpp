// gcDoUISetSprite - gcAll_psp.obj
//   0x00310560 gcDoUISetSprite::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
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

extern char gcDoUISetSpritevirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];

class gcDoUISetSprite {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
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
