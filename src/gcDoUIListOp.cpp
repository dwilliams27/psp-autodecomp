// gcDoUIListOp — gcAll_psp.obj
//   0x0030ce6c  New(cMemPool *, cBase *) static     (160B)
//   0x0030d024  Write(cFile &) const                (112B)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    gcAction(cBase *);
    void Write(cFile &) const;
};

void gcAction_ctor(gcAction *, cBase *);

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
};

void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoUIListOp : public gcAction {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char gcDoUIListOpvirtualtable[]; // 0x5A78

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ============================================================
// 0x0030d024 — Write(cFile &) const, 112B
// ============================================================
void gcDoUIListOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 12))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 24))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 28));
    wb.End();
}

// ============================================================
// 0x0030ce6c — New(cMemPool *, cBase *) static, 160B
// ============================================================
cBase *gcDoUIListOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIListOp *result = 0;
    gcDoUIListOp *obj = (gcDoUIListOp *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor((gcAction *)obj, parent);
        ((void **)obj)[1] = gcDoUIListOpvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 12), 1);
        *(int *)((char *)obj + 24) = (int)((unsigned int)obj | 1);
        *(int *)((char *)obj + 28) = 0;
        result = obj;
    }
    return (cBase *)result;
}
