#include "gcDoEntitySendMessage.h"
#include "cBase.h"

struct TypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoEntitySendPartialControllerMessage *, cFile &);

void gcDoEntitySendPartialControllerMessage::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);

    {
        char *typePtr = *(char **)((char *)this + 0x10);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x0C;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0x3C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x38;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0x54);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x50;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    ((cBaseArray *)((char *)this + 0x68))->Write(wb);

    int val = ((int *)this)[0x1C];
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
