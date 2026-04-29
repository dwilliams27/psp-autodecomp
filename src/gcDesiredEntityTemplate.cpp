class cBase;
class cFile;
class cMemPool;
class cObject;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
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

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F44C;

class gcDesiredEntityTemplate {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

cObject *gcDesiredEntityTemplate::GetObject(bool b) const {
    return Get(b);
}

void gcDesiredEntityTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();

    int val = *(int *)((const char *)this + 20);
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

const cType *gcDesiredEntityTemplate::GetType(void) const {
    if (D_0009F44C == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F44C = cType::InitializeType(0, 0, 0x1A6, D_0009F3F4,
                                           &gcDesiredEntityTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F44C;
}
