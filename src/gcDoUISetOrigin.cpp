#include "cBase.h"

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
};

class gcDoUISetOrigin {
public:
    void GetText(char *) const;
    void Write(cFile &) const;
};

void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUISetOrigin *, cFile &);

extern const char gcDoUISetOrigin_fmt[];
extern const char gcDoUISetOrigin_str2[];

// 0x0030fe5c, 112B
void gcDoUISetOrigin::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    wb.Write(*(int *)((char *)this + 0x18));
    wb.Write(*(bool *)((char *)this + 0x1C));
    wb.End();
}

// 0x00310110, 80B
void gcDoUISetOrigin::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetOrigin_fmt, local, gcDoUISetOrigin_str2);
}
