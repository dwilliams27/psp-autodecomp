// gcDesiredSoundData - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredSoundData::Write(cFile &) const                 @ 0x00129950
//   gcDesiredSoundData::New(cMemPool *, cBase *) static      @ 0x0026dd60
//   gcDesiredSoundData::GetType(void) const                  @ 0x0026de00
//   gcDesiredSoundData::~gcDesiredSoundData(void)            @ 0x0026dedc

class cBase;
class cFile;
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

extern char gcDesiredObjectT_gcDesiredSoundData_gcDesiredSoundDataHelper_eSoundData_virtualtable[];
extern char gcDesiredSoundDatavirtualtable[];
extern char cBaseclassdesc[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F468;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDesiredSoundData {
public:
    ~gcDesiredSoundData();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcDesiredSoundDatadtv, 0xd4\n");

// 0x0026dedc - gcDesiredSoundData::~gcDesiredSoundData(void)
gcDesiredSoundData::~gcDesiredSoundData() {
    *(char **)((char *)this + 4) =
        gcDesiredObjectT_gcDesiredSoundData_gcDesiredSoundDataHelper_eSoundData_virtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

// 0x00129950 - gcDesiredSoundData::Write(cFile &) const
void gcDesiredSoundData::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();
    wb.End();
}

// 0x0026dd60 - gcDesiredSoundData::New(cMemPool *, cBase *) static
cBase *gcDesiredSoundData::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredSoundData *result = 0;
    gcDesiredSoundData *obj =
        (gcDesiredSoundData *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] =
            gcDesiredObjectT_gcDesiredSoundData_gcDesiredSoundDataHelper_eSoundData_virtualtable;
        *(int *)((char *)obj + 12) = 7;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = gcDesiredSoundDatavirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0026de00 - gcDesiredSoundData::GetType(void) const
const cType *gcDesiredSoundData::GetType(void) const {
    if (D_0009F468 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F468 = cType::InitializeType(0, 0, 0x1FC, D_0009F3F4,
                                           &gcDesiredSoundData::New,
                                           0, 0, 0);
    }
    return D_0009F468;
}
