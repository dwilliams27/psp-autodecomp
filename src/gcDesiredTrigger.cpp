// gcDesiredTrigger - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredTrigger::Write(cFile &) const               @ 0x0012d6e8
//   gcDesiredTrigger::New(cMemPool *, cBase *) static    @ 0x00278c28
//   gcDesiredTrigger::GetType(void) const                @ 0x00278cc8

class cBase;
class cFile;
class cFileHandle;
class cReadBlock;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    int Read(cFile &, cMemPool *);
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
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char gcDesiredObjectT_gcDesiredTrigger_gcDesiredTriggerHelper_gcTrigger_virtualtable[];
extern char gcDesiredTriggervirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F4A8;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredTrigger {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcDesiredUIDialog {
public:
    int Read(cFile &, cMemPool *);
};

// 0x0012d6e8 - gcDesiredTrigger::Write(cFile &) const
void gcDesiredTrigger::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();
    wb.End();
}

// 0x0012d768 - gcDesiredTrigger::Read(cFile &, cMemPool *)
int gcDesiredTrigger::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int inner[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || ((gcDesiredObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    __0oKcReadBlockctR6FcFileUib(inner, *(cFile *)rb[0], 3, true);
    cFileSystem::Read(*(cFileHandle **)inner[0], (char *)this + 0x0C, 4);
    *(int *)((char *)this + 0x10) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x10);
        handle->Read(*(cReadBlock *)inner, cMemPool::GetPoolFromPtr(handle));
    }
    __0oKcReadBlockdtv(inner, 2);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x0012deb4 - gcDesiredUIDialog::Read(cFile &, cMemPool *)
int gcDesiredUIDialog::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int inner[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcDesiredObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    __0oKcReadBlockctR6FcFileUib(inner, *(cFile *)rb[0], 3, true);
    cFileSystem::Read(*(cFileHandle **)inner[0], (char *)this + 0x0C, 4);
    *(int *)((char *)this + 0x10) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x10);
        handle->Read(*(cReadBlock *)inner, cMemPool::GetPoolFromPtr(handle));
    }
    __0oKcReadBlockdtv(inner, 2);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x00278c28 - gcDesiredTrigger::New(cMemPool *, cBase *) static
cBase *gcDesiredTrigger::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredTrigger *result = 0;
    gcDesiredTrigger *obj =
        (gcDesiredTrigger *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] =
            gcDesiredObjectT_gcDesiredTrigger_gcDesiredTriggerHelper_gcTrigger_virtualtable;
        *(int *)((char *)obj + 12) = 1;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = gcDesiredTriggervirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00278cc8 - gcDesiredTrigger::GetType(void) const
const cType *gcDesiredTrigger::GetType(void) const {
    if (D_0009F4A8 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4A8 = cType::InitializeType(0, 0, 0x12E, D_0009F3F4,
                                           &gcDesiredTrigger::New,
                                           0, 0, 0);
    }
    return D_0009F4A8;
}
