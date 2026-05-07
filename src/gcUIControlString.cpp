// gcUIControlString — gcStringLValue-derived UI control with a
// gcDesiredUIWidgetHelper at offset 8 and two trailing int fields.
//
// Functions matched here:
//   gcUIControlString::AssignCopy(const cBase *)        @ 0x0028f3d0  ( 96B)
//   gcUIControlString::Write(cFile &) const             @ 0x0028f5e8  (112B)
//   gcUIControlString::~gcUIControlString(void)         @ 0x00290188  (100B)

class cBase;
class cFile;
class cMemPool;
class cType;
class gcUITextControl;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct gcDesiredUIWidgetHelper {
    int mField0;
    cHandle mField4;
    cHandle mField8;
    void GetText(char *) const;
    gcUITextControl *GetWidget(const cType *, bool) const;
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcStringValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    void Write(cFile &) const;
};

class gcStringLValue : public gcStringValue {
public:
    void Write(cFile &) const;
};

class gcUIControl {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcUITextControl {
public:
    static cBase *New(cMemPool *, cBase *);
    void SetText(const wchar_t *, int);
    void SetText(const gcStringValue *, int);
};

extern char gcStringLValuevirtualtable[];
extern char gcUIControlStringvirtualtable[];
extern char cBaseclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_0009F40C;
extern cType *D_0009F410;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F584;

class gcUIControlString : public gcStringLValue {
public:
    gcDesiredUIWidgetHelper mHelper;   // 0x08
    int mField14;                      // 0x14
    int mField18;                      // 0x18

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void Set(const wchar_t *) const;
    void Set(const gcStringValue *) const;
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcUIControlString();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

gcUIControlString *dcast(const cBase *);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::New(cMemPool *, cBase *)  @ 0x0028f430, 160B
// ─────────────────────────────────────────────────────────────────────────
cBase *gcUIControlString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *rec = (AllocEntry *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIControlString *result = 0;
    gcUIControlString *obj = (gcUIControlString *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)obj = parent;
        *(void **)((char *)obj + 4) = gcUIControlStringvirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::AssignCopy(const cBase *)  @ 0x0028f3d0, 96B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::AssignCopy(const cBase *base) {
    gcUIControlString *other = dcast(base);
    this->mHelper.mField0 = other->mHelper.mField0;
    this->mHelper.mField4 = other->mHelper.mField4;
    this->mHelper.mField8 = other->mHelper.mField8;
    this->mField14 = other->mField14;
    this->mField18 = other->mField18;
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::Write(cFile &) const  @ 0x0028f5e8, 112B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringLValue::Write(file);
    mHelper.Write(wb);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::Set(const wchar_t *) const  @ 0x0028fd0c, 452B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::Set(const wchar_t *text) const {
    gcDesiredUIWidgetHelper *helper =
        (gcDesiredUIWidgetHelper *)((char *)this + 8);

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

    gcUITextControl *control = helper->GetWidget(D_0009F410, true);
    if (control != 0) {
        int index = -1;
        if (mField14 == 1) {
            index = mField18;
        }
        control->SetText(text, index);
    }
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::Set(const gcStringValue *) const  @ 0x0028fed0, 452B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::Set(const gcStringValue *text) const {
    gcDesiredUIWidgetHelper *helper =
        (gcDesiredUIWidgetHelper *)((char *)this + 8);

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

    gcUITextControl *control = helper->GetWidget(D_0009F410, true);
    if (control != 0) {
        int index = -1;
        if (mField14 == 1) {
            index = mField18;
        }
        control->SetText(text, index);
    }
}

extern "C" int cStrFormat(char *, const char *, ...);
extern "C" int cStrAppend(char *, const char *, ...);

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::GetName(char *) const  @ 0x00290094, 116B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::GetName(char *dst) const {
    union {
        char text[0x100];
        char first;
    } buf;

    buf.first = 0;
    buf.text[0] = 0;
    mHelper.GetText(buf.text);
    cStrFormat(dst, (const char *)0x36E370, buf.text);
    if (mField14 == 1) {
        cStrAppend(dst, (const char *)0x36E378, mField18);
    }
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)  @ 0x00290108, 128B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::~gcUIControlString(void)  @ 0x00290188, 100B
// ─────────────────────────────────────────────────────────────────────────
gcUIControlString::~gcUIControlString() {
    *(void **)((char *)this + 4) = gcStringLValuevirtualtable;
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::GetType(void) const  @ 0x0028f4d0, 280B
// ─────────────────────────────────────────────────────────────────────────
const cType *gcUIControlString::GetType(void) const {
    if (D_0009F584 == 0) {
        if (D_0009F458 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                               0, 0, 0, 0);
        }
        D_0009F584 = cType::InitializeType(0, 0, 0x17A, D_0009F458,
                                           &gcUIControlString::New, 0, 0, 0);
    }
    return D_0009F584;
}
