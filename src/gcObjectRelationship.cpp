class cBase;
class cFile;
class cMemPool;

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

void cFile_SetCurrentPos(void *, unsigned int);

namespace cMemPool_ns {
    cMemPool *GetPoolFromPtr(const void *);
}

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
static cType *type_gcObjectRelationship; // 0x9F3F0

struct WriteEntry {
    short adj;
    short pad;
    void (*fn)(void *, cFile *);
};

struct ReadEntry {
    short adj;
    short pad;
    int (*fn)(void *, cFile *, cMemPool *);
};

class gcObjectRelationship {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

// 0x0011b454, 84B
void gcObjectRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    char *typeP = *(char **)((char *)this + 0xC);
    WriteEntry *e = (WriteEntry *)(typeP + 0x28);
    char *base = (char *)this + 8;
    e->fn(base + e->adj, *(cFile **)&wb._data[0]);
    wb.End();
}

// 0x0011b4a8, 208B
int gcObjectRelationship::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    cFile *f = *(cFile **)&rb._data[0];
    if ((unsigned int)rb._data[3] != 2) {
        cFile_SetCurrentPos(f, rb._data[1]);
        return 0;
    }
    char *vt = *(char **)((char *)this + 0xC);
    char *base = (char *)this + 8;
    ReadEntry *e = (ReadEntry *)(vt + 0x30);
    char *target = base + e->adj;
    cMemPool *pool2 = cMemPool_ns::GetPoolFromPtr(base);
    e->fn(target, f, pool2);
    return result;
}

// 0x0025eef4, 152B
const cType *gcObjectRelationship::GetType(void) const {
    if (type_gcObjectRelationship == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        type_gcObjectRelationship = cType::InitializeType(0, 0, 0x131, D_000385DC, 0, 0, 0, 0);
    }
    return type_gcObjectRelationship;
}
