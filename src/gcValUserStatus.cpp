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
class cType;

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
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct CloneEntry {
    short offset;
    short _pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

struct ReleaseEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

extern "C" {
    void gcValUserStatus__gcValUserStatus_cBaseptr(void *self, cBase *parent);
}

extern const char gcValUserStatus_fmt1[];
extern const char gcValUserStatus_fmt2[];
extern const char gcValUserStatus_str_open[];
extern const char gcValUserStatus_str_close[];

gcValUserStatus *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValUserStatus;

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

const cType *gcValUserStatus::GetType(void) const {
    if (!type_gcValUserStatus) {
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
        type_gcValUserStatus = cType::InitializeType(0, 0, 0x289, type_value,
                                                     gcValUserStatus::New,
                                                     0, 0, 0);
    }
    return type_gcValUserStatus;
}

void gcValUserStatus::AssignCopy(const cBase *base) {
    int temp_s2 = (int)dcast(base);
    int temp_s1 = (int)this + 0xC;

    *(int *)((char *)this + 8) = *(int *)(temp_s2 + 8);
    if (temp_s2 + 0xC != temp_s1) {
        int temp_a2 = *(int *)((char *)this + 0xC);
        int var_a1 = 1;
        int temp_a0 = temp_a2 & 1;

        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        if (var_a1 != 0) {
            int var_a1_2 = 0;
            int var_a2;

            if (temp_a0 != 0) {
                var_a1_2 = 1;
            }
            if (var_a1_2 != 0) {
                var_a2 = temp_a2 & ~1;
            } else {
                var_a2 = *(int *)temp_a2;
            }
            *(int *)((char *)this + 0xC) = var_a2 | 1;
            if (temp_a2 != 0) {
                void *temp_a2_2 = *(void **)(temp_a2 + 4);
                short temp_a3 = *(short *)((char *)temp_a2_2 + 0x50);
                void (*temp_a2_3)(void *, int) =
                    *(void (**)(void *, int))((char *)temp_a2_2 + 0x54);

                temp_a2_3((char *)temp_a2 + temp_a3, 3);
            }
        }

        int temp_a0_2 = *(int *)(temp_s2 + 0xC);
        int var_a0 = 1;
        if (temp_a0_2 & 1) {
            var_a0 = 0;
        }
        if (var_a0 != 0) {
            int temp_s2_2 = temp_a0_2;
            int temp_s1_2 = *(int *)(temp_s2_2 + 4) + 0x10;
            short temp_a1 = *(short *)temp_s1_2;
            cMemPool *temp_v0 = cMemPool::GetPoolFromPtr((char *)this + 0xC);
            int temp_a0_4 = *(int *)((char *)this + 0xC);
            int var_a2_2 = 0;

            temp_s2_2 += temp_a1;
            if (temp_a0_4 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                temp_a0_4 &= ~1;
            } else {
                temp_a0_4 = *(int *)temp_a0_4;
            }
            *(int *)((char *)this + 0xC) =
                (int)((CloneEntry *)temp_s1_2)
                    ->fn((void *)temp_s2_2, temp_v0, (cBase *)temp_a0_4);
        }
    }
    void *temp_v1 = &&dead;
    (void)temp_v1;
    return;
dead:
    goto dead;
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
