// gcDesiredUIDialog - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredUIDialog::Write(cFile &) const                 @ 0x0012de34
//   gcDesiredUIDialog::New(cMemPool *, cBase *) static      @ 0x00279be0
//   gcDesiredUIDialog::GetType(void) const                  @ 0x00279c80

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char gcDesiredObjectT_gcDesiredUIDialog_gcDesiredUIDialogHelper_gcUIDialog_virtualtable[];
extern char gcDesiredUIDialogvirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F4AC;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredUIDialog {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

// 0x0012de34 - gcDesiredUIDialog::Write(cFile &) const
void gcDesiredUIDialog::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();
    wb.End();
}

// 0x00279be0 - gcDesiredUIDialog::New(cMemPool *, cBase *) static
cBase *gcDesiredUIDialog::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredUIDialog *result = 0;
    gcDesiredUIDialog *obj =
        (gcDesiredUIDialog *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] =
            gcDesiredObjectT_gcDesiredUIDialog_gcDesiredUIDialogHelper_gcUIDialog_virtualtable;
        *(int *)((char *)obj + 12) = 1;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = gcDesiredUIDialogvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00279c80 - gcDesiredUIDialog::GetType(void) const
const cType *gcDesiredUIDialog::GetType(void) const {
    if (D_0009F4AC == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4AC = cType::InitializeType(0, 0, 0x16D, D_0009F3F4,
                                           &gcDesiredUIDialog::New,
                                           0, 0, 0);
    }
    return D_0009F4AC;
}
