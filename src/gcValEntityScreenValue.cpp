// gcValEntityScreenValue.cpp — decompiled from gcAll_psp.obj
//
//   0x0033c7c0  GetText(char *) const   (104B)
//
// Same pattern as gcValEntityPrimaryController::GetText: dispatches into the
// embedded gcDesiredEntity's text method via its cType, then appends two
// boilerplate strings.

class cBase;
class cFile;
class cMemPool;

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    char _p0[0x1C];
    cType *mParent;
    char _p1[0x8];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p2[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    int    _parent;
    cType *mType;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityScreenValue {
public:
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    gcValEntityScreenValue &operator=(const gcValEntityScreenValue &);
    static cBase *New(cMemPool *, cBase *);
};

void cStrAppend(char *, const char *, ...);

extern const char gcValEntityScreenValue_text_fmt[];     // 0x36F398
extern const char gcValEntityScreenValue_text_arg1[];    // 0x36DBB4
extern const char gcValEntityScreenValue_text_arg2[];    // 0x36DAF0

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityScreenValue;

void gcValEntityScreenValue::AssignCopy(const cBase *base) {
    const gcValEntityScreenValue *other = 0;

    if (base != 0) {
        if (!type_gcValEntityScreenValue) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_gcValEntityScreenValue = cType::InitializeType(
                0, 0, 0x16C, type_value, gcValEntityScreenValue::New,
                0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValEntityScreenValue;
        cType *type = entry->fn((char *)base + entry->offset);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
        fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValEntityScreenValue *)base;
        }
    }

    operator=(*other);
}

const cType *gcValEntityScreenValue::GetType(void) const {
    if (!type_gcValEntityScreenValue) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityScreenValue =
            cType::InitializeType(0, 0, 0x16C, type_value,
                                  gcValEntityScreenValue::New, 0, 0, 0);
    }
    return type_gcValEntityScreenValue;
}

void gcValEntityScreenValue::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const gcValue *)this)->Write(file);

    typedef void (*WriteFn)(void *, cFile *);

    const cTypeMethod *write0 =
        &((const gcDesiredObject *)((const char *)this + 8))->mType->write_m;
    char *base0 = (char *)this + 8;
    ((WriteFn)write0->fn)(base0 + write0->offset, wb.file);

    ((const cName *)((const char *)this + 0x34))->Write(wb);

    const cTypeMethod *write1 =
        &((const gcDesiredObject *)((const char *)this + 0x4C))->mType->write_m;
    char *base1 = (char *)this + 0x4C;
    ((WriteFn)write1->fn)(base1 + write1->offset, wb.file);

    wb.Write(*(const int *)((const char *)this + 0x84));
    wb.Write(*(const int *)((const char *)this + 0x88));
    ((const gcDesiredValue *)((const char *)this + 0x8C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x90))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x94))->Write(wb);
    wb.End();
}

// ── 0x0033c7c0 — GetText(char *) const, 104B ──
void gcValEntityScreenValue::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf, gcValEntityScreenValue_text_fmt);
    cStrAppend(buf, gcValEntityScreenValue_text_arg1,
                    gcValEntityScreenValue_text_arg2);
}
