class cBase;
class cFile;
class cMemPool;
class cType;
class eCamera;
class mOCS;
class mVec2;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
    gcUIWidget(cBase *);
    gcUIWidget &operator=(const gcUIWidget &);
    void Write(cFile &) const;
};

class gcUIControl : public gcUIWidget {
public:
    gcUIControl(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &,
                float, float, float, unsigned int);
};

template <class T>
T dcast(const cBase *);

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_0009F40C;

gcUIControl::gcUIControl(cBase *parent) : gcUIWidget(parent) {
    *(void **)((char *)this + 4) = (void *)0x388D60;
    *(int *)((char *)this + 0xB0) = 4;
    __vec_new((char *)this + 0xB4, 3, 4, (void (*)(void *))0x1E74A4);
    *(float *)((char *)this + 0xC0) = 32.0f;
    *(float *)((char *)this + 0xC4) = 32.0f;
}

const cType *gcUIControl::GetType(void) const {
    if (D_0009F40C == 0) {
        if (D_0009990C == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
            }
            D_0009990C =
                cType::InitializeType(0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F40C = cType::InitializeType(0, 0, 0x201, D_0009990C,
                                           gcUIControl::New, 0, 0, 0);
    }
    return D_0009F40C;
}

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

void gcUIControl::OnDraw(const eCamera &, const mOCS &, const mVec2 &,
                         const mVec2 &, float, float, float, unsigned int) {
}
