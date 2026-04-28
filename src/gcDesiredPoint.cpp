// gcDesiredPoint - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredPoint::Write(cFile &) const                 @ 0x001297bc
//   gcDesiredPoint::New(cMemPool *, cBase *) static      @ 0x0026cc78
//   gcDesiredPoint::GetType(void) const                  @ 0x0026cd18

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

extern char gcDesiredObjectT_gcDesiredPoint_gcDesiredPointHelper_ePoint_virtualtable[];
extern char gcDesiredPointvirtualtable[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F464;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDesiredPoint {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

// 0x001297bc - gcDesiredPoint::Write(cFile &) const
void gcDesiredPoint::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();
    wb.End();
}

// 0x0026cc78 - gcDesiredPoint::New(cMemPool *, cBase *) static
cBase *gcDesiredPoint::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredPoint *result = 0;
    gcDesiredPoint *obj = (gcDesiredPoint *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] =
            gcDesiredObjectT_gcDesiredPoint_gcDesiredPointHelper_ePoint_virtualtable;
        *(int *)((char *)obj + 12) = 7;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = gcDesiredPointvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0026cd18 - gcDesiredPoint::GetType(void) const
const cType *gcDesiredPoint::GetType(void) const {
    if (D_0009F464 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F464 = cType::InitializeType(0, 0, 0x14D, D_0009F3F4,
                                           &gcDesiredPoint::New,
                                           0, 0, 0);
    }
    return D_0009F464;
}
