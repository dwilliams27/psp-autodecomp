class cBase;
class cFile;
class cMemPool;

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoPlay2dSound {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcDoPlayMovie {
public:
    static cBase *New(cMemPool *, cBase *);
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

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern char gcDoPlay2dSoundvirtualtable[];
extern char gcDoPlayMovievirtualtable[];
extern char D_00000658[];
extern char D_00389800[];

cBase *gcDoPlay2dSound::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlay2dSound *result = 0;
    gcDoPlay2dSound *obj =
        (gcDoPlay2dSound *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlay2dSoundvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 2;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x14, obj);
        ((void **)obj)[6] = D_00000658;
        ((int *)obj)[8] = 7;
        ((int *)obj)[9] = 0;
        ((void **)obj)[6] = D_00389800;
        ((int *)obj)[10] = (int)((unsigned int)obj | 1);
        ((int *)obj)[11] = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoPlay2dSound::Write(cFile &file) const {
    cWriteBlock wb(file, 9);
    ((const gcAction *)this)->Write(file);
    wb.Write(((const int *)this)[3]);

    char *typeInfo = *(char **)((const char *)this + 0x18);
    WriteEntry *entry = (WriteEntry *)(typeInfo + 0x28);
    short off = entry->offset;
    void *base = (void *)((const char *)this + 0x14);
    entry->fn((char *)base + off, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x28))->Write(wb);
    wb.Write(((const int *)this)[4]);
    ((const gcDesiredValue *)((const char *)this + 0x2C))->Write(wb);
    wb.End();
}

cBase *gcDoPlayMovie::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayMovie *result = 0;
    gcDoPlayMovie *obj =
        (gcDoPlayMovie *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPlayMovievirtualtable;
        *(short *)((char *)obj + 0x20) = 0;
        *(short *)((char *)obj + 0x22) = 0;
        int value = (int)((unsigned int)obj | 1);
        *(char *)((char *)obj + 0x0C) = 0;
        *(int *)((char *)obj + 0x24) = value;
        *(char *)((char *)obj + 0x28) = 1;
        *(int *)((char *)obj + 0x2C) = 0;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        *(int *)((char *)obj + 0x38) = 0;
        *(int *)((char *)obj + 0x3C) = 0;
        *(int *)((char *)obj + 0x40) = value;
        *(int *)((char *)obj + 0x44) = value;
        *(int *)((char *)obj + 0x48) = value;
        *(int *)((char *)obj + 0x4C) = value;
        *(int *)((char *)obj + 0x50) = value;
        *(int *)((char *)obj + 0x54) = 0;
        result = obj;
    }
    return (cBase *)result;
}
