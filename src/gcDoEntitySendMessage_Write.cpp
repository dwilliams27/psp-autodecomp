#include "gcDoEntitySendMessage.h"
#include "cBase.h"

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void Write(int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoEntitySendMessage *, cFile &);

struct TypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

void gcDoEntitySendMessage::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction_Write(this, file);

    // Dispatch Write on subobject at offset 0x0C via type at 0x10
    {
        char *typePtr = *(char **)((char *)this + 0x10);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x0C;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    // Dispatch Write on subobject at offset 0x38 via type at 0x3C
    {
        char *typePtr = *(char **)((char *)this + 0x3C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x38;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    // cBaseArray::Write at offset 0x50
    ((cBaseArray *)((char *)this + 0x50))->Write(wb);

    // Encoded pointer at offset 0x58
    int val = ((int *)this)[0x16];
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

    wb.Write(((int *)this)[0x17]);
    wb.Write(((int *)this)[0x18]);

    wb.End();
}
