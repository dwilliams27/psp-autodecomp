// gcValEntityScreenValue.cpp — decompiled from gcAll_psp.obj
//
//   0x0033c7c0  GetText(char *) const   (104B)
//
// Same pattern as gcValEntityPrimaryController::GetText: dispatches into the
// embedded gcDesiredEntity's text method via its cType, then appends two
// boilerplate strings.

class cBase;
class cFile;

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78
};

class gcDesiredObject {
public:
    int    _parent;
    cType *mType;
};

class gcValEntityScreenValue {
public:
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);

extern const char gcValEntityScreenValue_text_fmt[];     // 0x36F398
extern const char gcValEntityScreenValue_text_arg1[];    // 0x36DBB4
extern const char gcValEntityScreenValue_text_arg2[];    // 0x36DAF0

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
