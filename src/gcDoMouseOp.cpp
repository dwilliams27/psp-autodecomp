// gcDoMouseOp - gcAll_psp.obj
//   0x002e7814 gcDoMouseOp::New(cMemPool *, cBase *) static
//   0x002e79f4 gcDoMouseOp::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    void Write(cFile &) const;
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x28];
    cTypeMethod write_m;
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

typedef void (*WriteFn)(void *, cFile *);

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);

extern char gcDoMouseOpvirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];

class gcDoMouseOp : public gcAction {
public:
    int mMode;
    unsigned int mFlags;
    cHandle mHandle;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

// 0x002e7814 - gcDoMouseOp::New(cMemPool *, cBase *) static
cBase *gcDoMouseOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoMouseOp *result = 0;
    gcDoMouseOp *obj = (gcDoMouseOp *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoMouseOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = -1;
        ((int *)obj)[5] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x18, obj);
        ((void **)obj)[7] = D_000006F8;
        ((int *)obj)[9] = 7;
        ((int *)obj)[10] = 0;
        ((void **)obj)[7] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002e79f4 - gcDoMouseOp::Write(cFile &) const
void gcDoMouseOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    wb.Write(*(int *)((const char *)this + 0x0C));
    wb.Write(*(unsigned int *)((const char *)this + 0x10));
    ((const cHandle *)((const char *)this + 0x14))->Write(wb);

    const cTypeMethod *entry =
        &((const cType *)*(void *const *)((const char *)this + 0x1C))->write_m;
    char *base = (char *)this + 0x18;
    ((WriteFn)entry->fn)(base + entry->offset, *(cFile **)&wb._data[0]);

    wb.End();
}
