class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

class gcDoTableQuery : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
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

typedef void (*WriteFn)(cBase *, cFile *);

struct TypeMethod {
    short offset;
    short pad;
    WriteFn fn;
};

void gcAction_gcAction(gcDoTableQuery *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);

extern char gcActionvirtualtable[];
extern char D_00000838[];

cBase *gcDoTableQuery::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTableQuery *result = 0;
    gcDoTableQuery *obj =
        (gcDoTableQuery *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcActionvirtualtable;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x0C, (cBase *)obj);
        ((void **)obj)[4] = D_00000838;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((int *)obj)[4] = 0x38A000;
        ((int *)obj)[8] = 0;
        ((gcDoTableQuery **)obj)[9] = obj;
        ((int *)obj)[10] = 0;
        ((gcDoTableQuery **)obj)[11] = obj;
        gcEvent_gcEvent((char *)obj + 0x30, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x4C, (cBase *)obj, 0);
        gcEvent_gcEvent((char *)obj + 0x68, (cBase *)obj, 0);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoTableQuery::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);

    TypeMethod *slot0 = (TypeMethod *)((char *)*(void **)((char *)this + 0x10) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x0C);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);
    ((const cBaseArray *)((const char *)this + 0x20))->Write(wb);
    ((const cBaseArray *)((const char *)this + 0x28))->Write(wb);

    TypeMethod *slot1 = (TypeMethod *)((char *)*(void **)((char *)this + 0x34) + 0x28);
    cBase *base1 = (cBase *)((char *)this + 0x30);
    slot1->fn((cBase *)((char *)base1 + slot1->offset), wb._file);
    TypeMethod *slot2 = (TypeMethod *)((char *)*(void **)((char *)this + 0x50) + 0x28);
    cBase *base2 = (cBase *)((char *)this + 0x4C);
    slot2->fn((cBase *)((char *)base2 + slot2->offset), wb._file);
    TypeMethod *slot3 = (TypeMethod *)((char *)*(void **)((char *)this + 0x6C) + 0x28);
    cBase *base3 = (cBase *)((char *)this + 0x68);
    slot3->fn((cBase *)((char *)base3 + slot3->offset), wb._file);
    wb.End();
}
