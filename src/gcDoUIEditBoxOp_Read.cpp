// ODR-WARNING: split-TU method addition. This file locally redeclares only the
// methods needed for gcDoUIEditBoxOp::Read so existing gcDoUIEditBoxOp siblings
// keep their original compile context.

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cFileHandle;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int pos;
    int _data[3];

    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDesiredUIWidgetHelper {
public:
    void Read(cReadBlock &);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class gcDoUIEditBoxOp : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1
int gcDoUIEditBoxOp::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if (rb._data[1] != 2 || gcAction::Read(file, pool) == 0) {
        rb.file->SetCurrentPos(rb.pos);
        return 0;
    }

    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Read(rb);
    cFileSystem::Read(*(cFileHandle **)rb.file, (char *)this + 0x18, 4);

    int value = *(int *)((char *)this + 0x1C);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int localValue;
    if (flag == 0) goto local_value;
    localValue = 0;
    goto local_done;

local_value:
    localValue = value;

local_done:
    int local = localValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    cBase *base;
    if (flag2 != 0) {
        base = (cBase *)(value & ~1);
    } else {
        base = *(cBase **)value;
    }

    rb.ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x1C), base,
                *(cBase **)&local);

    int stored = local;
    if (stored == 0) {
        stored = (int)base | 1;
        goto store;
    }
    __asm__ volatile("" ::: "memory");

store:
    *(int *)((char *)this + 0x1C) = stored;

    ((gcDesiredValue *)((char *)this + 0x20))->Read(rb);
    return result;
}
#pragma control sched=2
