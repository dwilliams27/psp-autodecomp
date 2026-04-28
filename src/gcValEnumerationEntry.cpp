// gcValEnumerationEntry — gcAll_psp.obj
//   0x00342118  gcValEnumerationEntry::Write(cFile &) const          (108B)
//   0x00341f4c  gcValEnumerationEntry::New(cMemPool *, cBase *)      (180B)
//
// Layout (size 0x20):
//   +0x00  cBase *parent
//   +0x04  primary classdesc pointer (cBase / cValue chain)
//   +0x08  self-pointer for secondary subobject (initialised to (cBase*)this)
//   +0x0C  secondary classdesc pointer (cType-like metadata table)
//   +0x10  unsigned char flag        (= 1)
//   +0x11  unsigned char pad         (= 0)
//   +0x14  int field                 (= 0)
//   +0x18  int field                 (= 0)
//   +0x1C  tagged self-handle        (= ((int)this) | 1)
//
// The Write entry is dispatched through (*(this+12)) + 0x28, which holds a
// {short offset, short pad, void (*fn)(void*, void*)} record.  The body
// passes (this+8 + offset, wb._data[0]) — i.e. the secondary subobject's
// adjusted address and the cFile pointer the cWriteBlock holds at +0.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
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

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValEnumerationEntry : public gcValue {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char gcValEnumerationEntry_inner_classdesc[];   // 0x000081D0

// ── 0x00342118 — gcValEnumerationEntry::Write(cFile &) const ──
void gcValEnumerationEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    char *desc = *(char **)((char *)this + 12);
    WriteEntry *e = (WriteEntry *)(desc + 0x28);
    short off = e->offset;
    char *sub = (char *)this + 8;
    e->fn(sub + off, (void *)wb._data[0]);
    wb.End();
}

// ── 0x00341f4c — gcValEnumerationEntry::New(cMemPool *, cBase *) static ──
cBase *gcValEnumerationEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcValEnumerationEntry *result = 0;
    int *p = (int *)entry->fn(base, 0x20, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(char **)((char *)p + 4)  = gcValEnumerationEntry_inner_classdesc;
        *(int *)((char *)p + 12) = 0x37E6A8;
        *(int **)((char *)p + 8) = p;
        *(int *)((char *)p + 12) = 0x388568;
        *((unsigned char *)p + 16) = 1;
        *((unsigned char *)p + 17) = 0;
        *(int *)((char *)p + 20) = 0;
        *(int *)((char *)p + 24) = 0;
        *(int *)((char *)p + 28) = (int)p | 1;
        result = (gcValEnumerationEntry *)p;
    }
    return (cBase *)result;
}
