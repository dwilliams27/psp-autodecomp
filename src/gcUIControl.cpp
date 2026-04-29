#include "gcUIControl.h"

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    int _data;
    void Write(cWriteBlock &) const;
};

class gcUIWidget {
public:
    gcUIWidget &operator=(const gcUIWidget &);
    void Write(cFile &) const;
};

template <class T>
T dcast(const cBase *);

void gcUIControl::AssignCopy(const cBase *base) {
    gcUIControl *other = dcast<gcUIControl *>(base);
    ((gcUIWidget *)this)->operator=(*(const gcUIWidget *)other);
    *(int *)((char *)this + 0xB0) = *(const int *)((const char *)other + 0xB0);

    int i = 0;
    int *dst = (int *)((char *)this + 0xB4);
    const int *src = (const int *)((const char *)other + 0xB4);
    do {
        i += 1;
        *dst = *src;
        dst += 1;
        src += 1;
    } while (i < 3);

    *(float *)((char *)this + 0xC0) = *(const float *)((const char *)other + 0xC0);
    *(float *)((char *)this + 0xC4) = *(const float *)((const char *)other + 0xC4);
}

void gcUIControl::Write(cFile &file) const {
    cWriteBlock wb(file, 8);
    ((const gcUIWidget *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xB0));
    wb.Write(2, (const float *)((const char *)this + 0xC0));
    wb.Write(3);

    int i = 0;
    const cHandle *handles = (const cHandle *)((const char *)this + 0xB4);
    do {
        handles[i].Write(wb);
        i += 1;
    } while (i < 3);

    wb.End();
}

void gcUIControl::OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int) {
}
