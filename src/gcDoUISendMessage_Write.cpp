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
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
};

void gcAction_Write(const gcDoUISendMessage *, cFile &);

void gcDoUISendMessage::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0x0C))->Write(wb);

    {
        char *typePtr = *(char **)((char *)this + 0x1C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x18;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    ((cBaseArray *)((char *)this + 0x30))->Write(wb);

    int val = ((int *)this)[0x38 / 4];
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
    wb.Write(((bool *)this)[0x3C]);
    wb.End();
}
