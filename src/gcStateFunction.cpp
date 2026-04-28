// gcStateFunction — named state function with embedded parameter sub-object

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *mFile;
    int _pad;
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cNamed {
public:
    void Write(cFile &) const;
};

struct ParamWriteVtableEntry {
    short adj;
    short pad;
    void (*fn)(void *self, cFile *file);
};

class gcStateFunction : public cNamed {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcStateFunction__gcStateFunction_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcStateFunction::Write(cFile &) const @ 0x0010973c ──
void gcStateFunction::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cNamed::Write(file);
    wb.Write(*(unsigned int *)((char *)this + 0x3c));
    char *vtable = *(char **)((char *)this + 0x24);
    ParamWriteVtableEntry *entry = (ParamWriteVtableEntry *)(vtable + 0x28);
    char *subThis = (char *)this + 0x20;
    void *adjThis = subThis + entry->adj;
    entry->fn(adjThis, wb.mFile);
    wb.End();
}

// ── gcStateFunction::New(cMemPool *, cBase *) static @ 0x00256830 ──
cBase *gcStateFunction::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStateFunction *result = 0;
    gcStateFunction *obj = (gcStateFunction *)rec->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        gcStateFunction__gcStateFunction_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
