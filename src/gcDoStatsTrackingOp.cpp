// gcDoStatsTrackingOp — gcAll_psp.obj
//   0x00303908  gcDoStatsTrackingOp::New(cMemPool *, cBase *) static
//   0x00303ac4  gcDoStatsTrackingOp::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoStatsTrackingOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

void gcAction_gcAction(gcDoStatsTrackingOp *, cBase *);
void gcAction_Write(const gcDoStatsTrackingOp *, cFile &);

extern char gcDoStatsTrackingOpvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoStatsTrackingOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoStatsTrackingOp *result = 0;
    gcDoStatsTrackingOp *obj = (gcDoStatsTrackingOp *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoStatsTrackingOpvirtualtable;
        ((int *)obj)[3] = 1;
        int tagged = (int)obj | 1;
        ((int *)obj)[4] = tagged;
        ((int *)obj)[5] = tagged;
        ((int *)obj)[6] = tagged;
        ((int *)obj)[7] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoStatsTrackingOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);

    int val = ((int *)this)[6];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);

    int val2 = ((int *)this)[7];
    int flag2 = 0;
    if (val2 & 1) {
        flag2 = 1;
    }
    cBase *ptr2;
    if (flag2 != 0) {
        ptr2 = 0;
    } else {
        ptr2 = (cBase *)val2;
    }
    wb.WriteBase(ptr2);
    wb.End();
}
