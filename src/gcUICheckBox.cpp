// gcUICheckBox -- decompiled from gcAll_psp.obj.
// Functions:
//   0x00139e94  ~gcUICheckBox(void)
//   0x0028ee64  New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolHelper {
public:
    static cMemPoolHelper *GetPoolFromPtr(const void *);
};

class gcUIControl {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcUITextControl {
public:
    gcUITextControl(cBase *);
    ~gcUITextControl();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class gcUICheckBox : public gcUITextControl {
public:
    gcUICheckBox(cBase *);
    ~gcUICheckBox();
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPoolHelper *pool = cMemPoolHelper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern char gcUICheckBoxvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_0009F40C;
extern cType *D_0009F410;
extern cType *D_0009F580;

gcUICheckBox::gcUICheckBox(cBase *parent) : gcUITextControl(parent) {
    *(void **)((char *)this + 4) = gcUICheckBoxvirtualtable;
    *(int *)((char *)this + 0x110) = 0;
    __vec_new((char *)this + 0x114, 3, 4, (void (*)(void *))0x1E74A4);
}

// SNC emits the null guard, base destructor call, and deleting tail.
gcUICheckBox::~gcUICheckBox() {
    *(void **)((char *)this + 4) = gcUICheckBoxvirtualtable;
}

const cType *gcUICheckBox::GetType(void) const {
    if (D_0009F580 == 0) {
        if (D_0009F410 == 0) {
            if (D_0009F40C == 0) {
                if (D_0009990C == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    D_0009990C = cType::InitializeType(
                        0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
                }
                D_0009F40C = cType::InitializeType(
                    0, 0, 0x201, D_0009990C, gcUIControl::New, 0, 0, 0);
            }
            D_0009F410 = cType::InitializeType(
                0, 0, 0x200, D_0009F40C, gcUITextControl::New, 0, 0, 0);
        }
        D_0009F580 = cType::InitializeType(
            0, 0, 0x7F, D_0009F410, gcUICheckBox::New, 0, 0, 0);
    }
    return D_0009F580;
}

void gcUICheckBox::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcUITextControl::Write(file);
    wb.Write(3);
    const cHandle *handles = (const cHandle *)((const char *)this + 0x114);
    for (int i = 0; i < 3; i++) {
        handles[i].Write(wb);
    }
    wb.End();
}

// gcUICheckBox::New(cMemPool *, cBase *) static @ 0x0028ee64
cBase *gcUICheckBox::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUICheckBox *result = 0;
    gcUICheckBox *obj = (gcUICheckBox *)rec->fn(base, 0x120, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUICheckBox(parent);
        result = obj;
    }
    return (cBase *)result;
}
