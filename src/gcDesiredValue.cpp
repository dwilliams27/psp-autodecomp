// gcDesiredValue.cpp — decompiled from gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredValue::Write(cWriteBlock &) const   @ 0x000e28c4 (104B)
//   gcDesiredValue::Read(cReadBlock &)           @ 0x000e292c (200B)
//
// Class layout:
//   [0x00] tagged-pointer "value" word.
//          - bit 0 set  ⇒ literal value (use raw bits with bit 0 cleared,
//                          and persist as raw|1)
//          - bit 0 clear ⇒ pointer to a cBase; vtable is *(int *)val

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

// ============================================================
// 0x000e28c4 — Write(cWriteBlock &) const
// ============================================================
void gcDesiredValue::Write(cWriteBlock &arg_wb) const {
    cWriteBlock wb(*reinterpret_cast<cFile *>(arg_wb._data[0]), 1);
    int val = *(int *)this;
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
    wb.End();
}

// ============================================================
// 0x000e292c — Read(cReadBlock &)
// ============================================================
void gcDesiredValue::Read(cReadBlock &arg_rb) {
    cReadBlock rb(*reinterpret_cast<cFile *>(arg_rb._data[0]), 1, true);
    int val = *(int *)this;
    int tag = val & 1;

    int flag1 = 0;
    if (tag != 0) flag1 = 1;

    cBase *outVal;
    if (flag1 != 0) {
        outVal = 0;
    } else {
        outVal = (cBase *)val;
    }
    __asm__ volatile("" : "+r"(outVal));
    cBase *sp14 = outVal;

    int flag2 = 0;
    if (tag != 0) flag2 = 1;

    int base;
    if (flag2 != 0) {
        base = val & ~1;
    } else {
        base = *(int *)val;
    }

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    rb.ReadBase(pool, (cBase *)base, sp14);

    int newVal;
    if (sp14 == 0) {
        newVal = base | 1;
    } else {
        newVal = (int)sp14;
    }
    *(int *)this = newVal;
}
