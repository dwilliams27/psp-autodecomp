// gcValUserStatus.cpp — decompiled from gcAll_psp.obj.
//
// Functions:
//   0x0012f9cc gcValUserStatus::Write(cFile &) const             100B
//   0x0027d634 gcValUserStatus::New(cMemPool *, cBase *) static  124B
//   0x001302e8 gcValUserStatus::GetText(char *) const            208B
//
// The constructor (0x0012fb0c, 40B) is matched in cXML_cNode_cNodecXML.cpp.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    char _pad[8];
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValUserStatus : public gcValue {
public:
    int   mPad8;     // +0x08
    int   mPadC;     // +0x0C  (gcDesiredValue header begins here)

    gcValUserStatus(cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void gcValUserStatus__gcValUserStatus_cBaseptr(void *self, cBase *parent);
}

extern const char gcValUserStatus_fmt1[];
extern const char gcValUserStatus_fmt2[];
extern const char gcValUserStatus_str_open[];
extern const char gcValUserStatus_str_close[];

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

// ── gcValUserStatus::Write @ 0x0012f9cc ──
void gcValUserStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(mPad8);
    ((gcDesiredValue *)((char *)this + 12))->Write(wb);
    wb.End();
}

// ── gcValUserStatus::New @ 0x0027d634 ──
cBase *gcValUserStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValUserStatus *result = 0;
    gcValUserStatus *obj = (gcValUserStatus *)rec->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        gcValUserStatus__gcValUserStatus_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUserStatus::GetText @ 0x001302e8 ──
typedef void (*GetTextVFn)(void *, char *);
struct GetTextThunk {
    short offset;
    short _pad;
    GetTextVFn fn;
};

static inline void DispatchGetText(cBase *receiver, char *buf) {
    char *classdesc = *(char **)((char *)receiver + 4);
    GetTextThunk *thunk = (GetTextThunk *)(classdesc + 0xD0);
    ((void (*)(void *, char *))thunk->fn)(
        (char *)receiver + thunk->offset, buf);
}

void gcValUserStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValUserStatus_fmt1, gcValUserStatus_fmt2);
    int val = mPad8;
    int notSix = (val != 6);
    notSix &= 0xFF;
    if (notSix != 0) {
        int valC = mPadC;
        int flag = 0;
        if (valC & 1) {
            flag = 1;
        }
        cBase *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (cBase *)valC;
        }
        if (ptr != 0) {
            DispatchGetText(ptr, buf);
        } else {
            cStrCat(buf, gcValUserStatus_str_open);
        }
    }
    cStrCat(buf, gcValUserStatus_str_close);
}
