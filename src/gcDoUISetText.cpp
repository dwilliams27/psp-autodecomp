// gcDoUISetText.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];
    gcDesiredUIWidgetHelper(int);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    char _pad[12];
    gcAction(cBase *);
    void Write(cFile &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
};

class gcDoUISetText : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcValObjectHasCategory : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern char cBaseclassdesc[];
extern char gcValuevirtualtable[];
extern char gcDoUISetTextvirtualtable[];
extern char gcValObjectHasCategoryvirtualtable[];

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

// -- gcDoUISetText::New(cMemPool *, cBase *) static @ 0x00313474 --
cBase *gcDoUISetText::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetText *result = 0;
    gcDoUISetText *obj = (gcDoUISetText *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetTextvirtualtable;
        *(int *)((char *)obj + 0x0C) = 0;
        *(int *)((char *)obj + 0x10) = 0;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0x14, 1);
        *(int *)((char *)obj + 0x20) = 0;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0x24) = 0;
        *(int *)((char *)obj + 0x28) = tagged;
        *(int *)((char *)obj + 0x2C) = tagged;
        *(int *)((char *)obj + 0x30) = tagged;
        *(int *)((char *)obj + 0x34) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// -- gcDoUISetText::Write(cFile &) const @ 0x00313644 --
void gcDoUISetText::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    gcAction::Write(file);
    wb.Write(*(int *)((const char *)this + 0x0C));
    wb.Write(*(int *)((const char *)this + 0x10));
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x14))->Write(wb);
    ((const cHandle *)((const char *)this + 0x20))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0x24));

    int val0 = *(int *)((const char *)this + 0x28);
    unsigned char flag0 = 0;
    int tag0 = val0 & 1;
    if (tag0 != 0) {
        flag0 = 1;
    }
    cBase *base0;
    if (flag0 == 0) {
        base0 = (cBase *)val0;
    } else {
        base0 = 0;
    }
    wb.WriteBase(base0);

    ((const gcDesiredValue *)((const char *)this + 0x30))->Write(wb);

    int val1 = *(int *)((const char *)this + 0x2C);
    unsigned char flag1 = 0;
    int tag1 = val1 & 1;
    if (tag1 != 0) {
        flag1 = 1;
    }
    cBase *base1;
    if (flag1 == 0) {
        base1 = (cBase *)val1;
    } else {
        base1 = 0;
    }
    wb.WriteBase(base1);

    wb.Write(*(int *)((const char *)this + 0x34));
    wb.End();
}

// -- gcValObjectHasCategory::New(cMemPool *, cBase *) static @ 0x00354890 --
cBase *gcValObjectHasCategory::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectHasCategory *result = 0;
    gcValObjectHasCategory *obj =
        (gcValObjectHasCategory *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValuevirtualtable;
        int tagged = (int)obj | 1;
        *(void **)((char *)obj + 0x10) = cBaseclassdesc;
        *(int *)((char *)obj + 0x08) = tagged;
        *(gcValObjectHasCategory **)((char *)obj + 0x0C) = obj;
        *(void **)((char *)obj + 0x10) = gcValObjectHasCategoryvirtualtable;
        *(unsigned char *)((char *)obj + 0x14) = 1;
        *(unsigned char *)((char *)obj + 0x15) = 0;
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = 0;
        *(int *)((char *)obj + 0x20) = tagged;
        result = obj;
    }
    return (cBase *)result;
}
