#include "cBase.h"

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
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

class gcDoUISetFocus {
public:
    void GetText(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
};

void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUISetFocus *, cFile &);
gcDoUISetFocus *dcast(const cBase *);

extern const char gcDoUISetFocus_fmt[];

// 0x0030ca34, 72B
void gcDoUISetFocus::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetFocus_fmt, local);
}

// 0x0030c730, 100B
void gcDoUISetFocus::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x18));
    wb.End();
}

// 0x0030c508, 120B
void gcDoUISetFocus::AssignCopy(const cBase *other) {
    gcDoUISetFocus *src = dcast(other);
    int *self_i = (int *)this;
    int *src_i = (int *)src;
    int v = self_i[2] & ~3;
    self_i[2] = v;
    self_i[2] = v | (src_i[2] & 3);
    self_i[3] = src_i[3];
    int *s1 = (int *)((char *)src + 0x10);
    int *d1 = (int *)((char *)this + 0x10);
    int *s2 = (int *)((char *)src + 0x14);
    int *d2 = (int *)((char *)this + 0x14);
    *d1 = *s1;
    *d2 = *s2;
    *(unsigned char *)((char *)this + 0x18) = *(const unsigned char *)((char *)src + 0x18);
}
