#include "gcUIWidget.h"

class cBase;
class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void Write(unsigned int);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    int _data[2];
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;

const cType *gcUIWidget::GetType(void) const {
    if (D_0009990C == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_0009990C = cType::InitializeType(0, 0, 0x84, D_000385E0,
                                           0, 0, 0, 0);
    }
    return D_0009990C;
}

void gcUIWidget::OnGetSize(mVec2 *, mVec2 *, unsigned int) const {
}

void gcUIWidget::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    ((const cNamed *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x24));
    wb.Write(*(const unsigned int *)((const char *)this + 0x28));

    int i = 0;
    const cHandle *handles = (const cHandle *)((const char *)this + 0x2C);
    do {
        handles[i].Write(wb);
        wb.Write(*(const unsigned int *)((const char *)&handles[i] + 4));
        i += 1;
    } while (i < 4);

    wb.Write(*(const int *)((const char *)this + 0x4C));
    wb.Write(2, (const float *)((const char *)this + 0x50));
    wb.Write(2, (const float *)((const char *)this + 0x58));
    wb.Write(2, (const float *)((const char *)this + 0x60));
    wb.Write(*(const float *)((const char *)this + 0x7C));
    wb.Write(*(const float *)((const char *)this + 0x80));
    ((const cBaseArray *)((const char *)this + 0x70))->Write(wb);
    wb.End();
}

gcUIWidget::gcUIWidget(cBase *parent) {
    *(cBase **)((char *)this + 0x00) = parent;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(char *)((char *)this + 0x08) = 0;
    *(void **)((char *)this + 0x04) = (void *)0x386F48;
    *(gcUIDialog **)((char *)this + 0x20) = GetParentDialog();
    *(int *)((char *)this + 0x28) = 0;
    *(unsigned int *)((char *)this + 0x24) = 0x02000003;
    __vec_new((char *)this + 0x2C, 4, 8, (void (*)(void *))0x23C4A0);

    *(int *)((char *)this + 0x4C) = 5;
    *(float *)((char *)this + 0x58) = 0.0f;
    *(float *)((char *)this + 0x50) = 0.0f;
    *(float *)((char *)this + 0x60) = 0.0f;
    *(float *)((char *)this + 0x5C) = 0.0f;
    *(int *)((char *)this + 0x68) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(float *)((char *)this + 0x54) = 0.0f;
    float one = 1.0f;
    *(gcUIWidget **)((char *)this + 0x74) = this;
    *(void **)((char *)this + 0x78) = (char *)this + 0x70;
    *(float *)((char *)this + 0x7C) = 0.0f;
    *(int *)((char *)this + 0x70) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x80) = one;
    *(float *)((char *)this + 0x84) = one;
    *(float *)((char *)this + 0x64) = 0.0f;
    *(float *)((char *)this + 0x8C) = one;
    *(char *)((char *)this + 0x90) = 0;
    *(float *)((char *)this + 0xA4) = 0.0f;
    *(float *)((char *)this + 0x9C) = 0.0f;
    *(float *)((char *)this + 0xA0) = 0.0f;
    *(float *)((char *)this + 0x98) = 0.0f;
    *(int *)((char *)this + 0x94) = 0;
    *(float *)((char *)this + 0x88) = one;

    struct Pair {
        int first;
        int second;
    };
    Pair pair;
    int zeroLocal;
    unsigned int i = 0;
    Pair *handle = (Pair *)((char *)this + 0x2C);
    int *zeroPtr = &zeroLocal;
    do {
        gcUIDialog *dialog = *(gcUIDialog **)((char *)this + 0x20);
        zeroLocal = 0;
        int first = 0;
        if (dialog != 0) {
            first = *(int *)((char *)dialog + 0x30);
        }
        pair.first = first;
        pair.second = *zeroPtr;
        __asm__ volatile("" ::: "memory");
        *handle = pair;
        i += 1;
        handle += 1;
    } while (i < 4);
}
