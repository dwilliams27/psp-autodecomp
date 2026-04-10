#include "gcDoPlayCinematic.h"
#include "cBase.h"

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cGUID {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoPlayCinematic *, cFile &);

void gcDoPlayCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((cHandle *)((char *)this + 0x10))->Write(wb);
    ((cHandle *)((char *)this + 0x14))->Write(wb);
    ((cGUID *)((char *)this + 0x18))->Write(wb);
    int val = ((int *)this)[8];
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
    ((cBaseArray *)((char *)this + 0x24))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x2C))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x30))->Write(wb);
    wb.End();
}
