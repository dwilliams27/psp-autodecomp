#include "gcDoReturn.h"
#include "cBase.h"

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoStateParentHandler *, cFile &);

void gcDoStateParentHandler::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((cBaseArray *)((char *)this + 0x0C))->Write(wb);
    int val = ((int *)this)[5];
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
