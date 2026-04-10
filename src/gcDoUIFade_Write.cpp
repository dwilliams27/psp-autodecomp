#include "gcDoUIFade.h"
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
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
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

void gcAction_Write(const gcDoUIFade *, cFile &);

void gcDoUIFade::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    wb.Write(((unsigned int *)this)[4]);

    {
        char *typePtr = *(char **)((char *)this + 0x18);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x14;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    ((cHandle *)((char *)this + 0x4C))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x50))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x54))->Write(wb);

    {
        char *typePtr = *(char **)((char *)this + 0x5C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x58;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    int val = ((int *)this)[0x74 / 4];
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
    ((cBaseArray *)((char *)this + 0x78))->Write(wb);
    wb.End();
}
